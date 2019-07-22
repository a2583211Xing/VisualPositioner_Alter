#include "CornerDetection.h"
#include <opencv2\imgproc.hpp>
#include "Utils.h"
#include <queue>
#include <algorithm>
#include <map>

using namespace std;
using namespace cv;



Mat ComputeCornerResponse(const Mat& cornerImage, int blockSize)
{
	double normalize_weight = 1.0 / (blockSize * blockSize);

	Mat integral_image;
	integral(cornerImage, integral_image, CV_32S);

	Mat corner_response = Mat::zeros(cornerImage.size(), CV_32F);
	for (int y = blockSize; y < cornerImage.rows - blockSize; y++)
	{
		for (int x = blockSize; x < cornerImage.cols - blockSize; x++)
		{
			Rect whole_area(x - blockSize, y - blockSize, blockSize * 2, blockSize * 2);
			Rect tl_box(x - blockSize, y - blockSize, blockSize, blockSize);
			Rect br_box(x, y, blockSize, blockSize);

			int whole_area_sum = GetRectSum(integral_image, whole_area);
			int tl_br_sum = GetRectSum(integral_image, tl_box) + GetRectSum(integral_image, br_box);
			
			corner_response.at<float>(y, x) = static_cast<float>(fabs(whole_area_sum - tl_br_sum - tl_br_sum));
		}
	}

	return corner_response;
}



Point DetectChessBoardCorner(const Mat& cornerImage, int blockSize, double cornerThreshold)
{
	Point corner_position;

	Mat corner_response = ComputeCornerResponse(cornerImage, blockSize);

	double max_response;
	Point max_location;
	minMaxLoc(corner_response, nullptr, &max_response, nullptr, &max_location);

	if (max_response >= cornerThreshold)
	{
		corner_position = max_location;
	}

	return corner_position;
}


void FindComponentMaximum(Mat& image, Point seed, float& maxValue, Point& maxLocation)
{
	queue<Point> component;
	component.push(seed);

	maxValue = numeric_limits<float>::min();
	Point current_point;
	float current_value;
	Point neighbor_point;
	float neighbor_value;

	static const vector<Point> neighbor_point_offset =
	{
		{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}
	};

	while (!component.empty())
	{
		current_point = component.front();
		component.pop();

		current_value = image.at<float>(current_point);
		image.at<float>(current_point) = 0;
		if (current_value > maxValue)
		{
			maxValue = current_value;
			maxLocation = current_point;
		}

		for (const auto& offset : neighbor_point_offset)
		{
			neighbor_point = current_point + offset;
			neighbor_value = image.at<float>(neighbor_point);
			if (neighbor_value >= 1e-5)
			{
				component.push(neighbor_point);
			}
		}
 	}
}


void ExtractCellBox(const vector<vector<Point>>& cellCornersInRow, int blockSize, vector<vector<Point>>& cellBoxes)
{
	for (int row = 0; row < cellCornersInRow.size() - 1; row++)
	{
		Point tl_corner, tr_corner, br_corner, bl_corner;
		for (int col = 0; col < cellCornersInRow[row].size() - 1; col++)
		{
			tl_corner = cellCornersInRow[row][col];
			tr_corner = cellCornersInRow[row][col+1];

			if (col < cellCornersInRow[row + 1].size())
			{
				bl_corner = cellCornersInRow[row + 1][col];
				if (tl_corner.x > bl_corner.x - blockSize
					&& tl_corner.x < bl_corner.x + blockSize
					&& col + 1 < cellCornersInRow[row + 1].size())
				{
					br_corner = cellCornersInRow[row + 1][col + 1];
					cellBoxes.push_back({ tl_corner, tr_corner, br_corner, bl_corner });
					continue;
				}
			}
			

			if (col - 1 >= 0 && col - 1 < cellCornersInRow[row + 1].size())
			{
				bl_corner = cellCornersInRow[row + 1][col - 1];
				if (tl_corner.x > bl_corner.x - blockSize
					&& tl_corner.x < bl_corner.x + blockSize
					&& col < cellCornersInRow[row + 1].size())
				{
					br_corner = cellCornersInRow[row + 1][col];
					cellBoxes.push_back({ tl_corner, tr_corner, br_corner, bl_corner });
					continue;
				}
			}
			

			if (col + 1 < cellCornersInRow[row + 1].size())
			{
				bl_corner = cellCornersInRow[row + 1][col + 1];
				if (tl_corner.x > bl_corner.x - blockSize
					&& tl_corner.x < bl_corner.x + blockSize
					&& col + 2 < cellCornersInRow[row + 1].size())
				{
					br_corner = cellCornersInRow[row + 1][col + 2];
					cellBoxes.push_back({ tl_corner, tr_corner, br_corner, bl_corner });
					continue;
				}
			}
		}
	}
}

vector<int> CalcHist16(const Mat& image)
{
	vector<int> hist(17, 0);
	for (int y = 0; y < image.rows; y++)
	{
		for (int x = 0; x < image.cols; x++)
		{
			hist[image.at<uchar>(y, x)]++;
		}
	}
	return hist;
}

void FindMax(const vector<int>& dataArray, int& maxValue, int& maxLoc)
{
	maxValue = numeric_limits<int>::min();
	for (int i = 0; i < dataArray.size(); i++)
	{
		if (dataArray[i] > maxValue)
		{
			maxValue = dataArray[i];
			maxLoc = i;
		}
	}
}

int IdentifyDigit(const Mat& digitImage, int bgIntensity)
{
	const vector<Rect> stroke_rois =
	{
		{19, 1, 27, 11},
		{1, 19, 11, 27},
		{53,19, 11, 27},
		{19, 53, 27, 11},
		{1, 71, 11, 27},
		{53,71, 11, 27},
		{19, 104, 27, 11}
	};
	const map<int, int> stroke_code =
	{
		{ 0, 1 << 6 },
		{ 1, 1 << 5 },
		{ 2, 1 << 4 },
		{ 3, 1 << 3 },
		{ 4, 1 << 2 },
		{ 5, 1 << 1 },
		{ 6, 1 << 0 }
	};
	const map<int, int> code2digit =
	{
		{ 0x77, 0 }, // 0b1110111
		{ 0x24, 1 }, // 0b0100100
		{ 0x5d, 2 }, // 0b1011101
		{ 0x5b, 3 }, // 0b1011011
		{ 0x3a, 4 }, // 0b0111010
		{ 0x6b, 5 }, // 0b1101011
		{ 0x6f, 6 }, // 0b1101111
		{ 0x52, 7 }, // 0b1010010
		{ 0x7f, 8 }, // 0b1111111
		{ 0x7b, 9 }  // 0b1111011
	};
	
	
	Mat stroke_image;
	int code = 0;
	for (int i = 0; i < stroke_rois.size(); i++)
	{
		stroke_image = digitImage(stroke_rois[i]);
		vector<int> hist = CalcHist16(stroke_image);

		if (hist[bgIntensity + 2] + hist[bgIntensity + 3] + hist[bgIntensity + 4] + hist[bgIntensity + 5]> stroke_rois[i].area()*0.2)
		{
			code += stroke_code.at(i);
		}
		else if (bgIntensity >= 5 && hist[bgIntensity - 2] + hist[bgIntensity - 3] + hist[bgIntensity - 4] + hist[bgIntensity - 5] > stroke_rois[i].area()*0.4)
		{
			code += stroke_code.at(i);
		}

		//if (bgIntensity <= 5) // dark background
		//{
		//	if (hist[bgIntensity + 2] + hist[bgIntensity + 3] + hist[bgIntensity + 4] + hist[bgIntensity + 5]> stroke_rois[i].area()*0.2)
		//	{
		//		code += stroke_code.at(i);
		//	}
		//}
		//else // bright background
		//{
		//	if (hist[bgIntensity - 2] + hist[bgIntensity - 3] + hist[bgIntensity - 4] + hist[bgIntensity - 5] > stroke_rois[i].area()*0.4)
		//	{
		//		code += stroke_code.at(i);
		//	}
		//}

		//int max_value, max_location;
		//FindMax(hist, max_value, max_location);
		//if (abs(max_location - bgIntensity) > 1) // stroke exists
		//{
		//	code += stroke_code.at(i);
		//}
	}

	int digit = -1;
	if (code2digit.count(code))
	{
		digit = code2digit.at(code);
	}
	else
	{
		digit = -1;
	}

	return digit;
}

Point GetBoardPosition(const Mat& boardImage, const vector<Point>& cellBoxCorners)
{
	const Size std_cell_size(666, 666);
	Point position;

	vector<Point2f> inputQuad =
	{
		cellBoxCorners[0],
		cellBoxCorners[1],
		cellBoxCorners[2],
		cellBoxCorners[3]
	};
	vector<Point2f> outputQuad =
	{
		{ 0.0f, 0.0f },
		{ static_cast<float>(std_cell_size.width - 1), 0.0f },
		{ static_cast<float>(std_cell_size.width - 1), static_cast<float>(std_cell_size.height - 1) },
		{0, static_cast<float>(std_cell_size.height - 1) }
	};

	Mat transform_matrix = getPerspectiveTransform(inputQuad, outputQuad);
	Mat cell_image;
	warpPerspective(boardImage, cell_image, transform_matrix, std_cell_size);

	Mat scaled_cell_image = cell_image / 16;
	vector<int> hist = CalcHist16(scaled_cell_image);

	int max_count = 0;
	int bg_intensity = 0;
	FindMax(hist, max_count, bg_intensity);

	const vector<Rect> digit_rois =
	{
		{173, 130, 67, 117},
		{424, 130, 67, 117},
		{173, 384, 67, 117},
		{424, 384, 67, 117}
	};
	
	vector<int> digits;
	for (const auto& roi : digit_rois)
	{
		int digit = IdentifyDigit(scaled_cell_image(roi), bg_intensity);
		if (digit == -1)
			return Point(-1, -1);
		digits.push_back(digit);

		/*rectangle(scaled_cell_image, roi, Scalar(16, 16, 16));*/
	}

	position.x = digits[0] * 10 + digits[1];
	position.y = digits[2] * 10 + digits[3];
	return position;
}


bool ScanCellCorners(const Mat& boardImage, int blockSize, double cornerThreshold,
	vector<Point>& cellCorners, vector<Point>& boardCorners)
{
	// 计算角点响应头，过滤低响应区域。
	Mat corner_response = ComputeCornerResponse(boardImage, blockSize);
	double potential_corner_threshold = cornerThreshold * 0.8;
	threshold(corner_response, corner_response, potential_corner_threshold, 255, CV_THRESH_TOZERO);
	
	// 缩小corner_response到原来1/10，获取粗略角点位置
	Mat zoomout_corner_response;
	resize(corner_response, zoomout_corner_response, corner_response.size() / 10);
	
	vector<Point> cell_corners;
	Point component_max_location;
	float component_max_value;

	for (int y = 0; y < zoomout_corner_response.rows; y++)
	{
		for (int x = 0; x < zoomout_corner_response.cols; x++)
		{
			if (zoomout_corner_response.at<float>(y, x) > 1e-5)
			{
				FindComponentMaximum(zoomout_corner_response, Point(x, y), component_max_value, component_max_location);
				if (component_max_value > potential_corner_threshold)
				{
					cell_corners.push_back(component_max_location * 10);
				}
			}
		}
	}

	// 根据粗略角点位置获取精确角点位置
	Rect image_roi(0, 0, corner_response.cols, corner_response.rows);
	for (auto& cell_corner : cell_corners)
	{
		Rect roi(cell_corner.x - blockSize * 2, cell_corner.y - blockSize * 2, blockSize * 4, blockSize * 4);
		roi = roi & image_roi;
		Point max_location;
		minMaxLoc(corner_response(roi), nullptr, nullptr, nullptr, &max_location);
		cell_corner = max_location + roi.tl();
	}

	// 按从上到下、从左到右排列角点
	auto comparator = [=](const Point& pa, const Point& pb)
	{
		if (pa.y < pb.y - blockSize)
		{
			return true;
		}
		else if (pa.y > pb.y + blockSize)
		{
			return false;
		}
		else
		{
			if (pa.x < pb.x - blockSize)
			{
				return true;
			}
			else if (pa.x > pb.x + blockSize)
			{
				return false;
			}
			else return false;
		}
	};
	sort(cell_corners.begin(), cell_corners.end(), comparator);
	
	// 角点分行
	vector<vector<Point>> cell_corners_in_row;
	for (size_t i = 0; i < cell_corners.size(); i++)
	{
		if (i == 0)
		{
			cell_corners_in_row.push_back({ cell_corners[i] });
		}
		else
		{
			if (cell_corners[i - 1].y < cell_corners[i].y - blockSize)
			{
				cell_corners_in_row.push_back({ cell_corners[i] });
			}
			else
			{
				cell_corners_in_row[cell_corners_in_row.size() - 1].push_back(cell_corners[i]);
			}
		}
		
	}


	// 提取方格cell
	vector<vector<Point>> cell_boxes;
	ExtractCellBox(cell_corners_in_row, blockSize, cell_boxes);

	// 提取每个方格左上角点标定版坐标系坐标
	cellCorners.clear();
	boardCorners.clear();
	for (const auto& cell_box_corners : cell_boxes)
	{
		cellCorners.push_back(cell_box_corners[0]);
		Point board_corner = GetBoardPosition(boardImage, cell_box_corners);
		if (board_corner == Point(-1, -1)) return false;
		boardCorners.push_back(board_corner);
	}

	return true;
}



vector<Point2d> RefineCellCorners(const Mat& boardImage, const vector<Point>& initialCorners, int blockSize, 
	double cornerThreshold)
{
	vector<Point2d> refinded_corners;
	for (const auto& corner : initialCorners)
	{
		Rect roi(corner.x - blockSize * 2, corner.y - blockSize * 2, blockSize * 4, blockSize * 4);
		Point refined_corner = DetectChessBoardCorner(boardImage(roi), blockSize, cornerThreshold);
		
		if (refined_corner == Point()) return vector<Point2d>();

		refinded_corners.push_back(refined_corner + roi.tl());
	}

	return refinded_corners;
}
