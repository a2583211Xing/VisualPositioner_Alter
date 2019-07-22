#include "CrossMarkDetection.h"
#include "Utils.h"
#include <opencv2\imgproc.hpp>

using namespace std;
using namespace cv;

/**************************************************************************************
 * Fn: Mat ComputeCrossResponse(const Mat& crossImage, int crossLength, int crossStripWidth)
 *
 * 产品十字架计算图
 *
 * Parameters:
 * crossImage - 	  包含一个十字架的图像
 * crossLength - 	  十字架的宽度
 * crossStripWidth -  Width of the cross strip.
 *
 * Returns: 计算结果图
****************************************************************************************/

Mat ComputeCrossResponse(const Mat& crossImage, int crossLength, int crossStripWidth)
{
	int half_cross_length = crossLength / 2;
	int half_strip_width = crossStripWidth / 2;
	int cross_length_rem2 = crossLength % 2;
	int strip_width_rem2 = crossStripWidth % 2;
	int tl_box_size = half_cross_length - half_strip_width;
	int br_box_size = tl_box_size + cross_length_rem2 - strip_width_rem2;

	int cross_weight = (crossLength - crossStripWidth) * (crossLength - crossStripWidth);
	int background_weight = crossLength * crossLength - cross_weight;
	double normalize_weight = 1.0 / (crossLength * crossLength);

	Mat integral_image;
	integral(crossImage, integral_image, CV_32S);

	Mat cross_response = Mat::zeros(crossImage.size(), CV_32F);
	for (int y = half_cross_length; y <= crossImage.rows - half_cross_length - cross_length_rem2; y++)
	{
		for (int x = half_cross_length; x <= crossImage.cols - half_cross_length - cross_length_rem2; x++)
		{
			Rect whole_area(x - half_cross_length, y - half_cross_length, crossLength, crossLength);
			Rect tl_box(x - half_cross_length, y - half_cross_length, tl_box_size, tl_box_size);
			Rect tr_box(x + half_strip_width + strip_width_rem2, y - half_cross_length, br_box_size, tl_box_size);
			Rect br_box(x + half_strip_width + strip_width_rem2, y + half_strip_width + strip_width_rem2, br_box_size, br_box_size);
			Rect bl_box(x - half_cross_length, y + half_strip_width + strip_width_rem2, tl_box_size, br_box_size);

			int whole_area_sum = GetRectSum(integral_image, whole_area);
			int background_sum = GetRectSum(integral_image, tl_box) + GetRectSum(integral_image, tr_box)
				+ GetRectSum(integral_image, br_box) + GetRectSum(integral_image, bl_box);

			cross_response.at<float>(y, x) = static_cast<float>(((whole_area_sum - background_sum) * cross_weight - background_sum * background_weight)
				* normalize_weight);
		}
	}

	return cross_response;
}

/**************************************************************************************
 * Fn: Point DetectCrossMark(const Mat& Image, int crossLength, int crossStripWidth,
 * double crossThreshold, const Rect& latentArea)
 *
 * 计算十字架中心图像坐标
 *
 * Parameters:
 * Image - 			  包含一个十字架的图像
 * crossLength - 	  Length of the cross.
 * crossStripWidth -  Width of the cross strip.
 * crossThreshold -   阈值，计算所需。
 * latentArea - 	 包含一个十字架的ROI在图像中的位置
 *
 * Returns: 中心点图像坐标
****************************************************************************************/

Point DetectCrossMark(const Mat& Image, int crossLength, int crossStripWidth, double crossThreshold,
	const Rect& latentArea)
{
	Point cross_center;

	Mat roi_image = Image;
	if (latentArea.area() > 0)
	{
		roi_image = Image(latentArea);
	}

	Mat cross_response = ComputeCrossResponse(roi_image, crossLength, crossStripWidth);

	double max_response;
	Point max_location;
	minMaxLoc(cross_response, nullptr, &max_response, nullptr, &max_location);

	if (max_response >= crossThreshold)
	{
		cross_center = max_location + latentArea.tl();
	}

	return cross_center;
}


Point SearchLeftBorder(const Mat& Image, int fgIntensity, Point currentLocation)
{
	int border_x = 0;
	for (int x = currentLocation.x; x < Image.cols - 5; x++)
	{
		if (Image.at<uchar>(currentLocation.y, x) >= fgIntensity &&
			Image.at<uchar>(currentLocation.y, x + 5) >= fgIntensity)
		{
			border_x = x;
			break;
		}
	}

	if (border_x == 0) return Point();
	Rect roi = { border_x - 10, currentLocation.y - 5, 20, 10 };
	Mat roi_image = Image(roi);
	Mat col_sum;
	reduce(roi_image, col_sum, 0, CV_REDUCE_AVG, CV_32S);

	int roi_border_x = 0;
	for (int x = 0; x < col_sum.cols; x++)
	{
		if (col_sum.at<int>(0, x) >= fgIntensity)
		{
			roi_border_x = x;
			break;
		}
	}

	if (roi_border_x == 0) return Point();
	return Point(roi.x + roi_border_x, roi.y + roi.height / 2);
}

Point SearchBottomBorder(const Mat& Image, int fgIntensity, Point currentLocation)
{
	int border_y = 0;
	for (int y = currentLocation.y; y >= 5; y--)
	{
		if (Image.at<uchar>(y, currentLocation.x) >= fgIntensity&&
			Image.at<uchar>(y - 5, currentLocation.x) >= fgIntensity)
		{
			border_y = y;
			break;
		}
	}

	if (border_y == 0) return Point();
	Rect roi(currentLocation.x - 5, border_y - 10, 10, 20 );
	Mat roi_image = Image(roi);
	Mat row_sum;
	reduce(roi_image, row_sum, 1, CV_REDUCE_AVG, CV_32S);

	int roi_border_y = 0;
	for (int y = row_sum.rows - 1; y >= 0; y--)
	{
		if (row_sum.at<int>(y, 0) >= fgIntensity)
		{
			roi_border_y = y;
			break;
		}
	}

	if (roi_border_y == 0) return Point();
	return Point(roi.x + roi.width / 2, roi.y + roi_border_y);
}


Point SearchRightBorder(const Mat& Image, int fgIntensity, Point currentLocation)
{
	int border_x = 0;
	for (int x = currentLocation.x; x >= 5; x--)
	{
		if (Image.at<uchar>(currentLocation.y, x) >= fgIntensity &&
			Image.at<uchar>(currentLocation.y, x - 5) >= fgIntensity)
		{
			border_x = x;
			break;
		}
	}

	if (border_x == 0) return Point();
	Rect roi = { border_x - 10, currentLocation.y - 5, 20, 10 };
	Mat roi_image = Image(roi);
	Mat col_sum;
	reduce(roi_image, col_sum, 0, CV_REDUCE_AVG, CV_32S);

	int roi_border_x = 0;
	for (int x = col_sum.cols - 1; x >= 0; x--)
	{
		if (col_sum.at<int>(0, x) >= fgIntensity)
		{
			roi_border_x = x;
			break;
		}
	}

	if (roi_border_x == 0) return Point();
	return Point(roi.x + roi_border_x, roi.y + roi.height / 2);
}



Point FindIntersection(Point laP0, Point laP1, Point lbP0, Point lbP1)
{
	cv::Point intersection_point;
	double a0 = laP0.y - laP1.y;
	double b0 = laP1.x - laP0.x;
	double c0 = laP0.x*laP1.y - laP1.x*laP0.y;
	double a1 = lbP0.y - lbP1.y;
	double b1 = lbP1.x - lbP0.x;
	double c1 = lbP0.x*lbP1.y - lbP1.x*lbP0.y;
	double denominator = a0*b1 - a1*b0;
	//如果两条直线没有交点，则直接返回（-1,-1）
	if (fabs(denominator) < 1e-5)
	{
		return Point(-1, -1);
	}
	else
	{
		intersection_point.x = static_cast<int>((b0 * c1 - b1 * c0) / denominator);
		intersection_point.y = static_cast<int>((a1 * c0 - a0 * c1) / denominator);
	}

	return intersection_point;
}



Rect FindCrossMarkRegion(const Mat& Image)
{	
	Rect cross_region;
	const int fg_intensity = 20;
	Point2d corner_cross_offset;
	Point vLinePa;
	Point vLinePb;
	Point hLinePa;
	Point hLinePb;

	Rect roi(0, 0, 10, 10);
	Mat roi_image = Image(roi);
	if (mean(roi_image).val[0] < 10) // 产品右下角视角
	{
		corner_cross_offset = { -347, -226 };

		vLinePa = SearchLeftBorder(Image, fg_intensity, Point(0, 5));
		vLinePb = SearchLeftBorder(Image, fg_intensity, Point(0, Image.rows * 3 / 7));
		hLinePa = SearchBottomBorder(Image, fg_intensity, Point(Image.cols - 5, Image.rows - 1));
		hLinePb = SearchBottomBorder(Image, fg_intensity, Point(Image.cols * 4 / 7, Image.rows - 1));
	}
	else
	{
		corner_cross_offset = { -804, -227 };
		vLinePa = SearchRightBorder(Image, fg_intensity, Point(Image.cols - 1, 5));
		vLinePb = SearchRightBorder(Image, fg_intensity, Point(Image.cols - 1, Image.rows * 3 / 7));
		hLinePa = SearchBottomBorder(Image, fg_intensity, Point(5, Image.rows - 1));
		hLinePb = SearchBottomBorder(Image, fg_intensity, Point(Image.cols * 3 / 7, Image.rows - 1));
	}

	if (vLinePa == Point() || vLinePb == Point() || hLinePa == Point() || hLinePb == Point()) return Rect();
	Point corner = FindIntersection(vLinePa, vLinePb, hLinePa, hLinePb);
	if (corner == Point(-1, -1)) return Rect();

	Point2d v_direction = vLinePb - vLinePa;
	v_direction /= norm(v_direction);
	Point2d h_direction = hLinePb - hLinePa;
	h_direction /= norm(h_direction);

	Point2d cross_center = Point2d(corner) + h_direction*corner_cross_offset.x + v_direction*corner_cross_offset.y;
	cross_region = { static_cast<int>(cross_center.x) - 80, static_cast<int>(cross_center.y) - 80, 160, 160 };

	return cross_region;
}
