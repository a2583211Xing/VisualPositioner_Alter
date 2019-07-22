#pragma once

#include <opencv2\core.hpp>

/*计算十字架中心点*/
cv::Point DetectCrossMark(const cv::Mat& Image, int crossLength, int crossStripWidth, double crossThreshold = 0,
	const cv::Rect& latentArea = cv::Rect());

/*求十字架区域*/
cv::Rect FindCrossMarkRegion(const cv::Mat& Image);

cv::Point SearchBottomBorder(const cv::Mat& Image, int fgIntensity, cv::Point currentLocation);

cv::Point SearchLeftBorder(const cv::Mat& Image, int fgIntensity, cv::Point currentLocation);

cv::Point SearchRightBorder(const cv::Mat& Image, int fgIntensity, cv::Point currentLocation);