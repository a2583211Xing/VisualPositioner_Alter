#pragma once

#include <vector>
#include <map>
#include <opencv2\core.hpp>



cv::Point DetectChessBoardCorner(const cv::Mat& cornerImage, int blockSize, double cornerThreshold = 0);

bool ScanCellCorners(const cv::Mat& boardImage, int blockSize, double cornerThreshold,
	std::vector<cv::Point>& cellCorners, std::vector<cv::Point>& boardCorners);


std::vector<cv::Point2d> RefineCellCorners(const cv::Mat& boardImage, const std::vector<cv::Point>& initialCorners, int blockSize = 40, double cornerThreshold = 0);
