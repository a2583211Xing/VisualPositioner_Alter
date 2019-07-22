#pragma once

#include <cmath>
#include <vector>
#include <opencv2\core.hpp>


inline double norm(double a, double b) { return sqrt(a*a + b*b); }

inline double norm(cv::Point2d p) { return sqrt(p.x*p.x + p.y*p.y); }

/*图像的区域和*/
inline int GetRectSum(const cv::Mat& IntegralImage, const cv::Rect& ROI)
{
	return IntegralImage.at<int>(ROI.y, ROI.x) + IntegralImage.at<int>(ROI.y + ROI.height, ROI.x + ROI.width)
		- IntegralImage.at<int>(ROI.y, ROI.x + ROI.width) - IntegralImage.at<int>(ROI.y + ROI.height, ROI.x);
}

cv::Point2d ProjectTranslation(cv::Point2d translation, cv::Point2d directionX, cv::Point2d directionY);

std::vector<cv::Point2d> Affine2dTransform(const std::vector<cv::Point2d>& inputPoints, const cv::Mat& transformMatrix);

cv::Point2d Affine2dTransform(cv::Point2d inputPoint, const cv::Mat& transformMatrix);

/*计算中垂线交点：旋转中心*/
cv::Point2d ComputeOptimalIntersection(const std::vector<cv::Vec4d>& lines);

