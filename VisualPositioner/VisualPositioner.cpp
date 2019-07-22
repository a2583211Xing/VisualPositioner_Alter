#include "VisualPositioner.h"
#include <opencv2\imgproc.hpp>
#include <opencv2\calib3d.hpp>
#include <fstream>
#include "Utils.h"
#include "CornerDetection.h"
#include "CrossMarkDetection.h"

using namespace std;
using namespace cv;
using json = nlohmann::json;

constexpr auto M_PI = 3.14159265358979323846;




string VisualPositioner::getCameraName(Camera camera)
{
	const static map<Camera, string> camera_name =
	{
		{ LeftCamera,"LeftCamera" },{ RightCamera,"RightCamera" }
	};

	return camera_name.at(camera);
}


Camera VisualPositioner::getCameraEnum(string camera)
{
	const static map< string, Camera > camera_enumeration =
	{
		{ "LeftCamera",LeftCamera },{ "RightCamera",RightCamera }
	};

	return camera_enumeration.at(camera);
}



bool VisualPositioner::loadSettings(string configFile)
{
	ifstream settings_file(configFile);
	if (settings_file.is_open())
	{
		settings_file >> settings;

		if (settings.count("ChessboardScale"))
		{
			boardScale = settings["ChessboardScale"].get<double>();
		}
		if (settings.count("CornerBlockSize"))
		{
			cornerBlockSize = settings["CornerBlockSize"].get<int>();
		}
		if (settings.count("CornerThreshold"))
		{
			cornerThreshold = settings["CornerThreshold"].get<double>();
		}
		if (settings.count("CrossMarkLength"))
		{
			crossMarkLength = settings["CrossMarkLength"].get<int>();
		}
		if (settings.count("CrossMarkStripWidth"))
		{
			crossMarkStripWidth = settings["CrossMarkStripWidth"].get<int>();
		}
		if (settings.count("CrossMarkThreshold"))
		{
			crossMarkThreshold = settings["CrossMarkThreshold"].get<double>();
		}
		if (settings.count("RotationCenter"))
		{
			rotationCenter.x = settings["RotationCenter"][0].get<double>();
			rotationCenter.y = settings["RotationCenter"][1].get<double>();
		}
		if (settings.count("TranslationDirectionX"))
		{
			translationDirectionX.x = settings["TranslationDirectionX"][0].get<double>();
			translationDirectionX.y = settings["TranslationDirectionX"][1].get<double>();
		}
		if (settings.count("TranslationDirectionY"))
		{
			translationDirectionY.x = settings["TranslationDirectionY"][0].get<double>();
			translationDirectionY.y = settings["TranslationDirectionY"][1].get<double>();
		}
		if (settings.count("Camera2BoardTransforms"))
		{
			if (settings["Camera2BoardTransforms"].count("LeftCamera"))
			{
				Mat left_camera_transform = Mat::zeros(Size(3, 2), CV_64FC1); 
				for (int y = 0; y < 2; y++)
				{
					for (int x = 0; x < 3; x++)
					{
						left_camera_transform.at<double>(y, x) = settings["Camera2BoardTransforms"]["LeftCamera"][y*3+x].get<double>();
					}
				}
				camera2BoardTransforms.insert({ getCameraEnum("LeftCamera"),left_camera_transform });
			}
			if (settings["Camera2BoardTransforms"].count("RightCamera"))
			{
				Mat right_camera_transform = Mat::zeros(Size(3, 2), CV_64FC1);
				for (int y = 0; y < 2; y++)
				{
					for (int x = 0; x < 3; x++)
					{
						right_camera_transform.at<double>(y, x) = settings["Camera2BoardTransforms"]["RightCamera"][y*3 + x].get<double>();
					}
				}
				camera2BoardTransforms.insert({ getCameraEnum("RightCamera"),right_camera_transform });
			}
		}
		if (settings.count("StandardPositions"))
		{
			if (settings["StandardPositions"].count("LeftCamera"))
			{
				Point2d left_camera_standard(0, 0);
				left_camera_standard.x = settings["StandardPositions"]["LeftCamera"][0].get<double>();
				left_camera_standard.y = settings["StandardPositions"]["LeftCamera"][1].get<double>();
				standardPositions.insert({ getCameraEnum("LeftCamera") ,left_camera_standard });
			}
			if (settings["StandardPositions"].count("RightCamera"))
			{
				Point2d right_camera_standard(0, 0);
				right_camera_standard.x = settings["StandardPositions"]["RightCamera"][0].get<double>();
				right_camera_standard.y = settings["StandardPositions"]["RightCamera"][1].get<double>();
				standardPositions.insert({ getCameraEnum("RightCamera") ,right_camera_standard });
			}
		}

		settings_file.close();
		errors.clear();
		return true;
	}

	errors.push_back(PositioningError::LoadingSettingsFailure);
	return false;
}



bool VisualPositioner::saveSettings(string configFile)
{
	ofstream settings_file(configFile);
	if (settings_file.is_open())
	{	
		settings_file << setw(4) << settings << endl;		

		return true;
	}

	errors.push_back(PositioningError::SaveSettingsFailure);
	return false;
}



bool VisualPositioner::registerCamera(Camera camera, const Mat& boardImage)
{
	if (errors.size()) return false;

	vector<Point> cell_corners;
	vector<Point> board_corners;
	bool corners_found = ScanCellCorners(boardImage, cornerBlockSize, cornerThreshold, cell_corners, board_corners);
	if (!corners_found) return false;

	vector<Point2d> scaled_board_corners;
	for (const auto& corner : board_corners)
	{
		scaled_board_corners.push_back(corner * boardScale);
	}

	/*计算精确标定板角点在图像上坐标*/
	vector<Point2d> refined_cell_corners;
	for (const auto& corner : cell_corners)
	{
		Rect roi(corner.x - cornerBlockSize * 2, corner.y - cornerBlockSize * 2, cornerBlockSize * 4, cornerBlockSize * 4);
		Point refined_corner = DetectChessBoardCorner(boardImage(roi), cornerBlockSize, cornerThreshold);
		if (refined_corner == Point())
		{
			errors.push_back(PositioningError::CornerDetectionFailure);
			return false;
		}
		refined_cell_corners.push_back(refined_corner + roi.tl());
	}

	Mat inliers;
	Mat affine2d_transform = estimateAffine2D(refined_cell_corners, scaled_board_corners, inliers, LMEDS);
	if (affine2d_transform.empty())
	{
		errors.push_back(PositioningError::CameraRegisterationFailure);
		return false;
	}
	if (camera2BoardTransforms.count(camera))
	{
		camera2BoardTransforms.at(camera) = affine2d_transform;
	}
	else
	{
		camera2BoardTransforms.insert({ camera, affine2d_transform });
	}

	
	{
		vector<double> affine2d_transform_data;
		for (int y = 0; y < affine2d_transform.rows; y++)
		{
			for (int x = 0; x < affine2d_transform.cols; x++)
			{
				affine2d_transform_data.push_back(affine2d_transform.at<double>(y, x));
			}
		}
		json j_affine2d_transforms(affine2d_transform_data);
		string camera_name = getCameraName(camera);
		settings["Camera2BoardTransforms"][camera_name] = j_affine2d_transforms;
	}

	return true;
}


vector<vector<Point2d>> VisualPositioner::ExtractCornerTracks(const vector<Mat>& images)
{
	vector<vector<Point2d>> corner_tracks;

	map<int, vector<Point>> tracks;
	for (const auto& image : images)
	{
		corner_tracks.push_back(vector<Point2d>());

		vector<Point> cell_corners;
		vector<Point> board_corners;
		bool succeed = ScanCellCorners(image, cornerBlockSize, cornerThreshold, cell_corners, board_corners);
		if (!succeed) return vector<vector<Point2d>>();

		for (int i = 0; i < board_corners.size(); i++)
		{
			int board_corner_key = (board_corners[i].y << 16) + board_corners[i].x;
			if (tracks.count(board_corner_key))
			{
				tracks[board_corner_key].push_back(cell_corners[i]);
			}
			else
			{
				tracks.insert({ board_corner_key, {cell_corners[i]} });
			}
		}
	}

	int track_size = static_cast<int>(images.size());
	for (const auto& track_entry : tracks)
	{
		if (track_entry.second.size() == track_size)
		{
			for (int i = 0; i < track_size; i++)
			{
				corner_tracks[i].push_back(track_entry.second[i]);
			}
		}
	}

	return corner_tracks;
}

vector<Vec4d> VisualPositioner::ConstructMidperpendiculars(Camera camera, const vector<Mat>& rotationBoardImages)
{
	vector<vector<Point2d>> corner_tracks = ExtractCornerTracks(rotationBoardImages);
	if (corner_tracks.empty())
	{
		errors.push_back(PositioningError::CornerDetectionFailure);
		return vector<Vec4d>();
	}

	vector<vector<Point2d>> projected_corner_tracks;
	for (int i = 0; i < rotationBoardImages.size(); i++)
	{
		vector<Point2d> corners = corner_tracks[i];

		if (camera2BoardTransforms.count(camera) == 0)
		{
			errors.push_back(PositioningError::CameraNotRegistered);
			return vector<Vec4d>();
		}
		corners = Affine2dTransform(corners, camera2BoardTransforms.at(camera));
		projected_corner_tracks.push_back(corners);
	}

	vector<Vec4d> lines;
	for (int i = 0; i < projected_corner_tracks.size() - 1; i++)
	{
		for (int j = 0; j < projected_corner_tracks[i].size(); j++)
		{
			Vec4d line;
			line.val[0] = (projected_corner_tracks[i][i].x + projected_corner_tracks[i + 1][i].x) / 2;
			line.val[1] = (projected_corner_tracks[i][i].y + projected_corner_tracks[i + 1][i].y) / 2;
			line.val[2] = projected_corner_tracks[i + 1][i].y - projected_corner_tracks[i][i].y;
			line.val[3] = -(projected_corner_tracks[i + 1][i].x - projected_corner_tracks[i][i].x);
			lines.push_back(line);
		}
	}
	return lines;
}

Point2d VisualPositioner::ComputeTranslationDirection(Camera camera, const vector<cv::Mat>& images)
{
	vector<vector<Point2d>> corner_tracks = ExtractCornerTracks(images);
	int count = 0;
	Point2d direction(0, 0);
	for (int i = 0; i < corner_tracks.size() - 1; i++)
	{
		for (int j = 0; j < corner_tracks[i].size(); j++, count++)
		{
			direction += corner_tracks[i + 1][j] - corner_tracks[i][j];
		}
	}

	if (count)
	{
		direction /= count;
	}
	else
	{
		errors.push_back(PositioningError::CornerDetectionFailure);
		return Point2d();
	}
	vector<Point2d> vector_2d = { {0, 0}, direction };
	if (camera2BoardTransforms.count(camera) == 0)
	{
		errors.push_back(PositioningError::CameraNotRegistered);
		return Point2d();
	}
	vector_2d = Affine2dTransform(vector_2d, camera2BoardTransforms.at(camera));

	direction = vector_2d[1] - vector_2d[0];
	return direction;
}




bool VisualPositioner::calibrateRotationCenter(const map<Camera, vector<Mat>>& imageSequences)
{
	if (errors.size()) return false;

	/*midperpendicular: 中垂线*/
	vector<Vec4d> midperpendiculars;
	for (const auto& image_seq : imageSequences)
	{
		vector<Vec4d> lines = ConstructMidperpendiculars(image_seq.first, image_seq.second);
		if (lines.empty()) return false;

		for (const auto& line : lines)
		{
			midperpendiculars.push_back(line);
		}
	}

	rotationCenter = ComputeOptimalIntersection(midperpendiculars);
	settings["RotationCenter"] = { rotationCenter.x, rotationCenter.y };

	return true;
}



bool VisualPositioner::registerStandardPosition(const std::map<Camera, cv::Mat>& produnctImages)
{
	if (errors.size()) return false;

	if (produnctImages.size() != 2)
	{
		errors.push_back(PositioningError::MissingInput);
		return false;
	}

	for (const auto& product_image_entry : produnctImages)
	{
		if (camera2BoardTransforms.count(product_image_entry.first) == 0)
		{
			errors.push_back(PositioningError::CameraNotRegistered);
			return false;
		}

		Mat product_image = product_image_entry.second;
		Rect cross_mark_region = FindCrossMarkRegion(product_image);
		Point cross_center = DetectCrossMark(product_image, crossMarkLength, crossMarkStripWidth, crossMarkThreshold, 
			cross_mark_region);
		if (cross_center == Point())
		{
			errors.push_back(PositioningError::CrossMarkDetectionFailure);
			return false;
		}
		standardPositions[product_image_entry.first] = Affine2dTransform(cross_center, 
			camera2BoardTransforms.at(product_image_entry.first));
	}
	//todo: different  transform
	for (auto a : standardPositions)
	{
		Point2d pt = a.second;		
		string camera_name = getCameraName(a.first);
		settings["StandardPositions"][camera_name] = { pt.x,pt.y };
	}
	
	return true;
}






bool VisualPositioner::calibrateTranslationDirection(const map<Camera, vector<Mat>>& imageSequencesX, const map<Camera, vector<Mat>>& imageSequencesY)
{
	if (errors.size()) return false;


	if (imageSequencesX.size() == 0 || imageSequencesY.size() == 0)
	{
		errors.push_back(PositioningError::MissingInput);
		return false;
	}
	
	translationDirectionX = Point2d();
	for (const auto& image_seq_entry : imageSequencesX)
	{
		Point2d translation_direction_x;
		if (image_seq_entry.second.size() == 0)
		{
			errors.push_back(PositioningError::MissingInput);
			return false;
		}

		translation_direction_x = ComputeTranslationDirection(image_seq_entry.first, image_seq_entry.second);
		translationDirectionX += translation_direction_x;
	}

	if (translationDirectionX == Point2d())
	{
		return false;
	}
	else
	{
		translationDirectionX /= norm(translationDirectionX);
	}


	translationDirectionY = Point2d();
	for (const auto& image_seq_entry : imageSequencesY)
	{
		Point2d translation_direction_y;
		if (image_seq_entry.second.size() == 0)
		{
			errors.push_back(PositioningError::MissingInput);
			return false;
		}

		translation_direction_y = ComputeTranslationDirection(image_seq_entry.first, image_seq_entry.second);
		translationDirectionY += translation_direction_y;
	}

	if (translationDirectionY == Point2d())
	{
		return false;
	}
	else
	{
		translationDirectionY /= norm(translationDirectionY);
	}

	settings["TranslationDirectionX"] =  { translationDirectionX.x, translationDirectionX.y };
	settings["TranslationDirectionY"] = { translationDirectionY.x, translationDirectionY.y };

	return true;
}



PositioningError VisualPositioner::getLastError()
{
	if (errors.empty()) return PositioningError::NoError;

	return errors.back();
}


void VisualPositioner::clearErrors()
{
	errors.clear();
}


bool VisualPositioner::computeAlignParameters(const map<Camera, Mat>& productImages, double& rotation, Point2d& translation)
{
	if (errors.size()) return false;

	/*寻找十字架中心并映射至标定板坐标*/
	map<Camera, Point2d> product_positions;
	for (const auto& product_image_entry : productImages)
	{
		if (camera2BoardTransforms.count(product_image_entry.first) == 0)
		{
			errors.push_back(PositioningError::CameraNotRegistered);
			return false;
		}

		Mat product_image = product_image_entry.second;
		Rect cross_mark_region = FindCrossMarkRegion(product_image);
		Point cross_center = DetectCrossMark(product_image, crossMarkLength, crossMarkStripWidth, crossMarkThreshold, 
			cross_mark_region);
		if (cross_center == Point())
		{
			errors.push_back(PositioningError::CrossMarkDetectionFailure);
			return false;
		}
		product_positions[product_image_entry.first] = Affine2dTransform(cross_center, 
			camera2BoardTransforms.at(product_image_entry.first));
	}

	/*计算旋转弧度*/
	if (standardPositions.size() != 2)
	{
		errors.push_back(PositioningError::StandardPositionNotRegistered);
		return false;
	}
	Vec2d standard_orientation(standardPositions[Camera::RightCamera].x - standardPositions[Camera::LeftCamera].x,
		standardPositions[Camera::RightCamera].y - standardPositions[Camera::LeftCamera].y);
	Vec2d product_orientation(product_positions[Camera::RightCamera].x - product_positions[Camera::LeftCamera].x,
		product_positions[Camera::RightCamera].y - product_positions[Camera::LeftCamera].y);
	
	//两个向量点乘/两个向量的模乘积？
	//s·p=|s||p|*cosθ； arccosθ=弧度；角度=弧度*180/PI；
	double inner_product = (standard_orientation.val[0] * product_orientation.val[0] + standard_orientation.val[1] 
		* product_orientation.val[1]) /(norm(standard_orientation.val[0], standard_orientation.val[1])
			* norm(product_orientation.val[0], product_orientation.val[1]));

	double angle = acos(inner_product) * 180 / M_PI;
	if ((rotationCenter.y > standardPositions[Camera::LeftCamera].y && product_positions[Camera::LeftCamera].x > standardPositions[Camera::LeftCamera].x)
		|| (rotationCenter.y < standardPositions[Camera::LeftCamera].y && product_positions[Camera::LeftCamera].x < standardPositions[Camera::LeftCamera].x))
	{
		angle = -angle;
	}
	rotation = angle;


	if (rotationCenter == Point2d())
	{
		errors.push_back(PositioningError::RotationCenterNotCalibrated);
		return false;
	}
	Point2d standard_middle_point;
	standard_middle_point.x = (standardPositions[Camera::LeftCamera].x + standardPositions[Camera::RightCamera].x) / 2
		- rotationCenter.x;
	standard_middle_point.y = (standardPositions[Camera::LeftCamera].y + standardPositions[Camera::RightCamera].y) / 2 
		- rotationCenter.y;
	Point2d product_middle_point;
	product_middle_point.x = (product_positions[Camera::LeftCamera].x + product_positions[Camera::RightCamera].x) / 2
		- rotationCenter.x;
	product_middle_point.y = (product_positions[Camera::LeftCamera].y + product_positions[Camera::RightCamera].y) / 2 
		- rotationCenter.y;

	/*弧度*/
	double angle_radian = rotation * M_PI / 180;
	Point2d rotated_product_middle_point = {
		product_middle_point.x * cos(angle_radian) - product_middle_point.y * sin(angle_radian),
		product_middle_point.x * sin(angle_radian) + product_middle_point.y * cos(angle_radian)
	};

	translation = ProjectTranslation(standard_middle_point - rotated_product_middle_point, translationDirectionX, translationDirectionY);
	return true;
}
