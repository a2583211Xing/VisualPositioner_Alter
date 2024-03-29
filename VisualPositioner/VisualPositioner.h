﻿/**************************************************************************************************
// Copyright(C), 2015-2019, MISV Machine Vision Lab.
//
// file:	VisualPositioner.h
//
// summary:	声明视觉定位算法类VisualPositioner
 **************************************************************************************************/

#pragma once

#include "Enums.h"
#include <string>
#include <vector>
#include <map>
#include <opencv2\core.hpp>
#include <nlohmann\json.hpp>
#include <opencv2/opencv.hpp>


class VisualPositioner
{
public:
	VisualPositioner() { errors.push_back(PositioningError::SettingsNotLoaded); }

	std::string getCameraName(Camera camera);
	Camera getCameraEnum(std::string camera);
	bool loadSettings(std::string configFile);
	bool saveSettings(std::string configFile);

	/*计算标定板精确角点并计算相机坐标系-标定板坐标系之仿射矩阵*/
	bool registerCamera(Camera camera, const cv::Mat& boardImage);

	/*标定旋转中心*/
	bool calibrateRotationCenter(const std::map<Camera, std::vector<cv::Mat>>& imageSequences);

	/*计算标准位置之十字架中心点，并转换至标定板坐标系*/
	bool registerStandardPosition(const std::map<Camera, cv::Mat>& productImages);

	bool calibrateTranslationDirection(const std::map<Camera, std::vector<cv::Mat>>& imageSequencesX, const std::map<Camera, std::vector<cv::Mat>>& imageSequencesY);

	PositioningError getLastError();

	void clearErrors();

	/*计算矫正参数：rotation，translation(平移量，方向）*/
	bool computeAlignParameters(const std::map<Camera, cv::Mat>& productImages, double& rotation, cv::Point2d& translation);



public:
    std::vector<cv::Rect> camer1Line1ROI;
    int camer1line1Direction;
    int camer1line1Threshold;

    std::vector<cv::Rect> camer1Line2ROI;
    int camer1line2Direction;
    int camer1line2Threshold;

    std::vector<cv::Rect> camer2Line1ROI;
    int camer2line1Direction;
    int camer2line1Threshold;

    std::vector<cv::Rect> camer2Line2ROI;
    int camer2line2Direction;
    int camer2line2Threshold;

    cv::Point2d ComputeLeftCamerIntersection(cv::Mat &image);
    cv::Point2d ComputeRightCamerIntersection(cv::Mat &image);

    cv::Mat ComputeLeftCamerIntersectionShow(cv::Mat &image);
    cv::Mat ComputeRightCamerIntersectionShow(cv::Mat &image);

protected:

	/*提取旋转角点轨迹*/
	std::vector<std::vector<cv::Point2d>> ExtractCornerTracks(const std::vector<cv::Mat>& images);

	/*计算中垂线集*/
	std::vector<cv::Vec4d> ConstructMidperpendiculars(Camera camera, const std::vector<cv::Mat>& rotationBoardImages);

	cv::Point2d ComputeTranslationDirection(Camera camera, const std::vector<cv::Mat>& images);

    cv::Point SearchPoint(const cv::Mat& image, const cv::Rect& roi,int direction,int threshold);

    cv::Point2d ComputeOptimalIntersection(const std::vector<cv::Vec4d>& lines);

    std::vector<cv::Point2d> LinearSearch(const cv::Mat& image, cv::Rect roi,int threshold = 40, int num = 10, Direction dire = LeftToRight,  Polarity polar = WhiteToBlack, );

    cv::Point SearchPoint2(const cv::Mat& image, const cv::Rect& roi,int threshold, Direction dire,Polarity polar);

    cv::Mat linear(std::vector<cv::Point>& points );





	nlohmann::json settings;
	/*类成员变量：标定板比例*/
	double boardScale;
	/*类成员变量：角点ROI边长*/
	int cornerBlockSize;
	/*类成员变量：*/
	double cornerThreshold;
	/*类成员变量：十字架ROI边长*/
	int crossMarkLength;
	/*类成员变量：*/
	int crossMarkStripWidth;
	/*类成员变量：*/
	double crossMarkThreshold;

	/*类成员变量：*/
	std::vector<PositioningError> errors;

	/*类成员变量：两个相机坐标系-标定板坐标系之仿射矩阵*/
	std::map<Camera, cv::Mat> camera2BoardTransforms;

	/*类成员变量：标定板上之旋转中心*/
	cv::Point2d rotationCenter;

	/*类成员变量：*/
	cv::Point2d translationDirectionX;
	cv::Point2d translationDirectionY;

	/*类成员变量：十字架中心在标定板坐标系中的坐标*/
	std::map<Camera, cv::Point2d> standardPositions;

};
