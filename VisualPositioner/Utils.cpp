#include "Utils.h"

using namespace std;
using namespace cv;

/**************************************************************************************
 * Fn: vector<Point2d> Affine2dTransform(const vector<Point2d>& inputPoints,
 * const Mat& transformMatrix)
 *
 * 一个坐标系的点集映射到另一个坐标系
 *
 * Parameters:
 * inputPoints - 	  原坐标系之点集
 * transformMatrix -   两个坐标系之间的仿射矩阵
 *
 * Returns:  点集在另一个坐标系中的坐标集
****************************************************************************************/

vector<Point2d> Affine2dTransform(const vector<Point2d>& inputPoints, const Mat& transformMatrix)
{
	Mat input_points_homo(Size(static_cast<int>(inputPoints.size()), 3), CV_64F);
	for (int i = 0; i < inputPoints.size(); i++)
	{
		input_points_homo.at<double>(0, i) = inputPoints[i].x;
		input_points_homo.at<double>(1, i) = inputPoints[i].y;
		input_points_homo.at<double>(2, i) = 1;
	}
	Mat output_points_homo = transformMatrix * input_points_homo;

	vector<Point2d> output_points;
	for (int i = 0; i < output_points_homo.cols; i++)
	{
		output_points.push_back(Point2d(output_points_homo.at<double>(0, i), output_points_homo.at<double>(1, i)));
	}
	return output_points;
}

/**************************************************************************************
 * 上面的重载函数，输入参数只接受一个点，输出也是一个点
****************************************************************************************/

Point2d Affine2dTransform(Point2d inputPoint, const Mat& transformMatrix)
{
	Mat input_point_homo(Size(1, 3), CV_64F);
	input_point_homo.at<double>(0, 0) = inputPoint.x;
	input_point_homo.at<double>(1, 0) = inputPoint.y;
	input_point_homo.at<double>(2, 0) = 1;

	Mat output_point_homo = transformMatrix * input_point_homo;
	return Point2d(output_point_homo.at<double>(0, 0), output_point_homo.at<double>(1, 0));
}

Point2d ComputeOptimalIntersection(const vector<Vec4d>& lines)
{
	Mat A = Mat::zeros(Size(2, 2), CV_64F);
	Mat B = Mat::zeros(Size(1, 2), CV_64F);

	for (auto line : lines)
	{
		Mat x0(Size(1, 2), CV_64F);
		x0.at<double>(0, 0) = line.val[0];
		x0.at<double>(1, 0) = line.val[1];
		double vnorm = norm(line.val[2], line[3]);
		line.val[2] /= vnorm;
		line.val[3] /= vnorm;
		Mat D(Size(2, 2), CV_64F);
		D.at<double>(0, 0) = line.val[2] * line.val[2] - 1;
		D.at<double>(0, 1) = line.val[2] * line.val[3];
		D.at<double>(1, 0) = D.at<double>(0, 1);
		D.at<double>(1, 1) = line.val[3] * line.val[3] - 1;
		A = A + D;
		B = B + (D * x0);
	}
	Mat p = A.inv() * B;

	return Point2d(p.at<double>(0, 0), p.at<double>(1, 0));
}

Point2d ProjectTranslation(Point2d translation, Point2d directionX, cv::Point2d directionY)
{
	Point2d projectedTranslation;
	projectedTranslation.x = translation.ddot(directionX);
	projectedTranslation.y = translation.ddot(directionY);

	return projectedTranslation;
}