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

        if (settings.count("Camer1Line1ROI"))
        {
            camer1Line1ROI.clear();
            camer1Line1ROI.push_back(cv::Rect());
            camer1Line1ROI.push_back(cv::Rect());
            if (settings["Camer1Line1ROI"].count("Rect1"))
            {
                camer1Line1ROI.at(0).x      = settings["Camer1Line1ROI"]["Rect1"][0].get<int>();
                camer1Line1ROI.at(0).y      = settings["Camer1Line1ROI"]["Rect1"][1].get<int>();
                camer1Line1ROI.at(0).width  = settings["Camer1Line1ROI"]["Rect1"][2].get<int>();
                camer1Line1ROI.at(0).height = settings["Camer1Line1ROI"]["Rect1"][3].get<int>();



            }
            if (settings["Camer1Line1ROI"].count("Rect2"))
            {
                camer1Line1ROI.at(1).x      = settings["Camer1Line1ROI"]["Rect2"][0].get<int>();
                camer1Line1ROI.at(1).y      = settings["Camer1Line1ROI"]["Rect2"][1].get<int>();
                camer1Line1ROI.at(1).width  = settings["Camer1Line1ROI"]["Rect2"][2].get<int>();
                camer1Line1ROI.at(1).height = settings["Camer1Line1ROI"]["Rect2"][3].get<int>();
            }
        }

        if (settings.count("Camer1line1Direction"))
        {
            camer1line1Direction = settings["Camer1line1Direction"].get<int>();
        }
        if (settings.count("Camer1line1Threshold"))
        {
            camer1line1Threshold = settings["Camer1line1Threshold"].get<int>();
        }


        if (settings.count("Camer1Line2ROI"))
        {
            camer1Line2ROI.clear();
            camer1Line2ROI.push_back(cv::Rect());
            camer1Line2ROI.push_back(cv::Rect());
            if (settings["Camer1Line2ROI"].count("Rect1"))
            {
                camer1Line2ROI.at(0).x      = settings["Camer1Line2ROI"]["Rect1"][0].get<int>();
                camer1Line2ROI.at(0).y      = settings["Camer1Line2ROI"]["Rect1"][1].get<int>();
                camer1Line2ROI.at(0).width  = settings["Camer1Line2ROI"]["Rect1"][2].get<int>();
                camer1Line2ROI.at(0).height = settings["Camer1Line2ROI"]["Rect1"][3].get<int>();
            }
            if (settings["Camer1Line2ROI"].count("Rect2"))
            {
                camer1Line2ROI.at(1).x      = settings["Camer1Line2ROI"]["Rect2"][0].get<int>();
                camer1Line2ROI.at(1).y      = settings["Camer1Line2ROI"]["Rect2"][1].get<int>();
                camer1Line2ROI.at(1).width  = settings["Camer1Line2ROI"]["Rect2"][2].get<int>();
                camer1Line2ROI.at(1).height = settings["Camer1Line2ROI"]["Rect2"][3].get<int>();
            }
        }

        if (settings.count("Camer1line2Direction"))
        {
            camer1line2Direction = settings["Camer1line2Direction"].get<int>();
        }
        if (settings.count("Camer1line2Threshold"))
        {
            camer1line2Threshold = settings["Camer1line2Threshold"].get<int>();
        }

        //camer2
        if (settings.count("Camer2Line1ROI"))
        {
            camer2Line1ROI.clear();
            camer2Line1ROI.push_back(cv::Rect());
            camer2Line1ROI.push_back(cv::Rect());
            if (settings["Camer2Line1ROI"].count("Rect1"))
            {
                camer2Line1ROI.at(0).x      = settings["Camer2Line1ROI"]["Rect1"][0].get<int>();
                camer2Line1ROI.at(0).y      = settings["Camer2Line1ROI"]["Rect1"][1].get<int>();
                camer2Line1ROI.at(0).width  = settings["Camer2Line1ROI"]["Rect1"][2].get<int>();
                camer2Line1ROI.at(0).height = settings["Camer2Line1ROI"]["Rect1"][3].get<int>();
            }
            if (settings["Camer2Line1ROI"].count("Rect2"))
            {
                camer2Line1ROI.at(1).x      = settings["Camer2Line1ROI"]["Rect2"][0].get<int>();
                camer2Line1ROI.at(1).y      = settings["Camer2Line1ROI"]["Rect2"][1].get<int>();
                camer2Line1ROI.at(1).width  = settings["Camer2Line1ROI"]["Rect2"][2].get<int>();
                camer2Line1ROI.at(1).height = settings["Camer2Line1ROI"]["Rect2"][3].get<int>();
            }
        }

        if (settings.count("Camer2line1Direction"))
        {
            camer2line1Direction = settings["Camer2line1Direction"].get<int>();
        }
        if (settings.count("Camer2line1Direction"))
        {
            camer2line1Threshold = settings["Camer2line1Threshold"].get<int>();
        }


        if (settings.count("Camer2Line2ROI"))
        {
            camer2Line2ROI.clear();
            camer2Line2ROI.push_back(cv::Rect());
            camer2Line2ROI.push_back(cv::Rect());
            if (settings["Camer2Line2ROI"].count("Rect1"))
            {
                camer2Line2ROI.at(0).x      = settings["Camer2Line2ROI"]["Rect1"][0].get<int>();
                camer2Line2ROI.at(0).y      = settings["Camer2Line2ROI"]["Rect1"][1].get<int>();
                camer2Line2ROI.at(0).width  = settings["Camer2Line2ROI"]["Rect1"][2].get<int>();
                camer2Line2ROI.at(0).height = settings["Camer2Line2ROI"]["Rect1"][3].get<int>();
            }
            if (settings["Camer2Line2ROI"].count("Rect2"))
            {
                camer2Line2ROI.at(1).x      = settings["Camer2Line2ROI"]["Rect2"][0].get<int>();
                camer2Line2ROI.at(1).y      = settings["Camer2Line2ROI"]["Rect2"][1].get<int>();
                camer2Line2ROI.at(1).width  = settings["Camer2Line2ROI"]["Rect2"][2].get<int>();
                camer2Line2ROI.at(1).height = settings["Camer2Line2ROI"]["Rect2"][3].get<int>();
            }
        }

        if (settings.count("Camer2line2Direction"))
        {
            camer2line2Direction = settings["Camer2line2Direction"].get<int>();
        }
        if (settings.count("Camer2line2Threshold"))
        {
            camer2line2Threshold = settings["Camer2line2Threshold"].get<int>();
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
    //camer1
    vector<int> rect;
    rect.push_back(camer1Line1ROI.at(0).x);
    rect.push_back(camer1Line1ROI.at(0).y);
    rect.push_back(camer1Line1ROI.at(0).width);
    rect.push_back(camer1Line1ROI.at(0).height);
    settings["Camer1Line1ROI"]["Rect1"] = rect;

    rect.clear();
    rect.push_back(camer1Line1ROI.at(1).x);
    rect.push_back(camer1Line1ROI.at(1).y);
    rect.push_back(camer1Line1ROI.at(1).width);
    rect.push_back(camer1Line1ROI.at(1).height);
    settings["Camer1Line1ROI"]["Rect2"] = rect;

    rect.clear();
    rect.push_back(camer1Line2ROI.at(0).x);
    rect.push_back(camer1Line2ROI.at(0).y);
    rect.push_back(camer1Line2ROI.at(0).width);
    rect.push_back(camer1Line2ROI.at(0).height);
    settings["Camer1Line2ROI"]["Rect1"] = rect;

    rect.clear();
    rect.push_back(camer1Line2ROI.at(1).x);
    rect.push_back(camer1Line2ROI.at(1).y);
    rect.push_back(camer1Line2ROI.at(1).width);
    rect.push_back(camer1Line2ROI.at(1).height);
    settings["Camer1Line2ROI"]["Rect2"] = rect;

    settings["Camer1line1Direction"] = camer1line1Direction;
    settings["Camer1line1Threshold"] = camer1line1Threshold;
    settings["Camer1line2Direction"] = camer1line2Direction;
    settings["Camer1line2Threshold"] = camer1line2Threshold;



    //camer2
    rect.clear();
    rect.push_back(camer2Line1ROI.at(0).x);
    rect.push_back(camer2Line1ROI.at(0).y);
    rect.push_back(camer2Line1ROI.at(0).width);
    rect.push_back(camer2Line1ROI.at(0).height);
    settings["Camer2Line1ROI"]["Rect1"] = rect;

    rect.clear();
    rect.push_back(camer2Line1ROI.at(1).x);
    rect.push_back(camer2Line1ROI.at(1).y);
    rect.push_back(camer2Line1ROI.at(1).width);
    rect.push_back(camer2Line1ROI.at(1).height);
    settings["Camer2Line1ROI"]["Rect2"] = rect;

    rect.clear();
    rect.push_back(camer2Line2ROI.at(0).x);
    rect.push_back(camer2Line2ROI.at(0).y);
    rect.push_back(camer2Line2ROI.at(0).width);
    rect.push_back(camer2Line2ROI.at(0).height);
    settings["Camer2Line2ROI"]["Rect1"] = rect;

    rect.clear();
    rect.push_back(camer2Line2ROI.at(1).x);
    rect.push_back(camer2Line2ROI.at(1).y);
    rect.push_back(camer2Line2ROI.at(1).width);
    rect.push_back(camer2Line2ROI.at(1).height);
    settings["Camer2Line2ROI"]["Rect2"] = rect;

    settings["Camer2line1Direction"] = camer2line1Direction;
    settings["Camer2line1Threshold"] = camer2line1Threshold;
    settings["Camer2line2Direction"] = camer2line2Direction;
    settings["Camer2line2Threshold"] = camer2line2Threshold;


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
        int LX = corner.x - cornerBlockSize * 2;
        int LY = corner.y - cornerBlockSize * 2;
        int RX = LX + cornerBlockSize * 4;
        int RY = LY + cornerBlockSize * 4;
        if (LX < 0)
        {
            LX = 0;
        }
        if (LY < 0)
        {
            LY = 0;
        }
        if (RX > boardImage.cols)
        {
            RX = boardImage.cols;
        }

        if (RY > boardImage.rows)
        {
            RY = boardImage.rows;
        }

        //Rect roi(corner.x - cornerBlockSize * 2, corner.y - cornerBlockSize * 2, cornerBlockSize * 4, cornerBlockSize * 4);
        Rect roi(Point(LX,LY),Point(RX,RY));
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

Point VisualPositioner::SearchPoint(const Mat &image, const Rect &roi, int direction, int threshold)
{
    Mat roi_image = image(roi);

    Point border_point(0, 0);

    if(direction<2)
    {
        Mat col_sum;
        reduce(roi_image, col_sum, 0, CV_REDUCE_AVG);
        //从左到右
        if(direction == 0 )
            for (int i = 1; i < col_sum.cols - 1; i++)
            {

                if ( abs(col_sum.at<uchar>(0, i + 1) - col_sum.at<uchar>(0, i - 1)) >= threshold)
                {
                    border_point.x = roi.x + i;
                    break;
                }
            }

        //从右到左
        if(direction == 1 )
            for (int i = col_sum.cols - 2; i > 0; i--)
            {
                if ( abs (col_sum.at<uchar>(0, i - 1) - col_sum.at<uchar>(0, i + 1) ) >= threshold)
                {
                    border_point.x = roi.x + i;
                    break;
                }
            }


        if (border_point.x == 0)
        {
            border_point.x =roi.x + col_sum.cols;
        }
        border_point.y = roi.y + (roi.height / 2);
    }
    else
    {
        Mat row_sum;
        reduce(roi_image, row_sum, 1, CV_REDUCE_AVG);
        //从上到下
        if(direction == 2 )
            for (int i = 1; i < row_sum.rows -1; i++)
            {
                if (abs (row_sum.at<uchar>(i + 1, 0) - row_sum.at<uchar>(i - 1, 0)) >= threshold)
                {
                    border_point.y = roi.y + i;
                    break;
                }
            }

        //从下到上
        if(direction == 3 )
            for (int i = row_sum.rows - 2; i > 0; i--)
            {
                if (abs(row_sum.at<uchar>(i - 1, 0) - row_sum.at<uchar>(i + 1, 0)) >= threshold)
                {
                    border_point.y = roi.y + i;
                    break;
                }
            }

        if (border_point.y == 0)
        {
            border_point.y = roi.y + row_sum.rows;
        }
        border_point.x = roi.x + (roi.width / 2);
    }




    return border_point;
}

Point2d VisualPositioner::ComputeOptimalIntersection(const std::vector<Vec4d> &lines)
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

std::vector<Point2d> VisualPositioner::LinearSearch(const Mat &image, Rect roi, int threshold, int num, Direction dire, VisualPositioner::Polarity polar)
{

    std::vector<cv::Point> fitPoint;
    int step =0;
    if(dire<2)
    {

        step = roi.height/num;

        for(int i=0; i<num; i++)
        {
            int lpY = roi.Y + i*step;

            cv::Point tempP = SearchPoint2(image,cv::Rect(roi.x,lpY,roi.width,step),threshold,dire,polar);
            if(tempP != Point(-1,-1))
            {
                fitPoint.push_back(tempP);
            }
        }
    }
    else
    {
        step = roi.width/num;
        for(int i=0; i<num; i++)
        {
            int lpX = roi.y + i*step;
            cv::Point tempP = SearchPoint2(image,cv::Rect(lpX,roi.y,step,roi.height),threshold,dire,polar);
            if(tempP != Point(-1,-1))
            {
                fitPoint.push_back(tempP);
            }
        }
    }

    cv::Mat coefficient = linear(fitPoint);

    if(********)




}

Point VisualPositioner::SearchPoint2(const Mat &image, const Rect &roi, int threshold, Direction dire, Polarity polar)
{
    Mat roi_image = image(roi);
    Point border_point(-1, -1);
    if(dire < 2 )
    {
        Mat col_sum;
        reduce(roi_image, col_sum, 0, CV_REDUCE_AVG);
        if(dire == 0 )
        {

            //从左到右
            for (int i = 1; i < col_sum.cols - 1; i++)
            {

                if ( polar*(col_sum.at<uchar>(0, i + 1) - col_sum.at<uchar>(0, i - 1)) >= threshold)
                {
                    border_point.x = roi.x + i;
                    break;
                }
            }

        }
        if(dire == 1 )
        {

            //从右到左
            for (int i = col_sum.cols - 2; i > 0; i--)
            {
                if ( polar* (col_sum.at<uchar>(0, i - 1) - col_sum.at<uchar>(0, i + 1) ) >= threshold)
                {
                    border_point.x = roi.x + i;
                    break;
                }
            }
        }
        if (border_point.x != -1)
        {
            border_point.y = roi.y + (roi.height / 2);
        }


    }
    else
    {
        Mat row_sum;
        reduce(roi_image, row_sum, 1, CV_REDUCE_AVG);
        if(dire == 2 )
        {

            //从上到下
            for (int i = 1; i < row_sum.rows -1; i++)
            {
                if (polar* (row_sum.at<uchar>(i + 1, 0) - row_sum.at<uchar>(i - 1, 0)) >= threshold)
                {
                    border_point.y = roi.y + i;
                    break;
                }
            }
        }
        if(dire == 3 )
        {

            //从下到上
            for (int i = row_sum.rows - 2; i > 0; i--)
            {
                if (polar*(row_sum.at<uchar>(i - 1, 0) - row_sum.at<uchar>(i + 1, 0)) >= threshold)
                {
                    border_point.y = roi.y + i;
                    break;
                }
            }

        }
        if (border_point.y != -1)
        {
            border_point.x = roi.x + (roi.width / 2);
        }


    }

    return border_point;
}

Mat VisualPositioner::linear(std::vector<Point> &points)
{
    Mat Y = Mat::zeros(Size(points.size(), 1), CV_64F);
    Mat X = Mat::ones(Size(points.size(), 2), CV_64F);

    for( int i=0; i<points.size(); i++ )
    {
        X.at<double>(i,0) = points.at(i).x;
        Y.at<double>(i,0) = points.at(i).y;
    }

    return X.inv() * Y;
}

Point2d VisualPositioner::ComputeLeftCamerIntersection(Mat &image)
{
    Mat image_shown;
    cvtColor(image, image_shown, CV_GRAY2BGR);

    vector<Point> line1_points;
    for (const auto& roi : camer1Line1ROI)
    {


        Point border_point = SearchPoint(image, roi,camer1line1Direction,camer1line1Threshold);
        if (border_point != Point(0, 0))
        {
            line1_points.push_back(border_point);
        }
    }

    line(image_shown, line1_points[0], line1_points[1], Scalar(0, 0, 255));

    vector<Point> line2_points;
    for (const auto& roi : camer1Line2ROI)
    {


        Point border_point = SearchPoint(image, roi,camer1line2Direction,camer1line2Threshold);
        if (border_point != Point(0, 0))
        {
            line2_points.push_back(border_point);
        }
    }

    line(image_shown, line2_points[0], line2_points[1], Scalar(0, 0, 255));

    vector<Vec4d> line1_lin2_intersection_lines = {
        {
            static_cast<double>(line1_points[0].x),  static_cast<double>(line1_points[0].y),
            static_cast<double>(line1_points[1].x - line1_points[0].x),  static_cast<double>(line1_points[1].y - line1_points[0].y)
        },
        {
            static_cast<double>(line2_points[0].x),  static_cast<double>(line2_points[0].y),
            static_cast<double>(line2_points[1].x - line2_points[0].x),  static_cast<double>(line2_points[1].y - line2_points[0].y)
        }
    };





    return ComputeOptimalIntersection(line1_lin2_intersection_lines);


}

Point2d VisualPositioner::ComputeRightCamerIntersection(Mat &image)
{
    vector<Point> line1_points;
    for (const auto& roi : camer2Line1ROI)
    {


        Point border_point = SearchPoint(image, roi,camer2line1Direction,camer2line1Threshold);
        if (border_point != Point(0, 0))
        {
            line1_points.push_back(border_point);
        }
    }

    vector<Point> line2_points;
    for (const auto& roi : camer2Line2ROI)
    {


        Point border_point = SearchPoint(image, roi,camer2line2Direction,camer2line2Threshold);
        if (border_point != Point(0, 0))
        {
            line2_points.push_back(border_point);
        }
    }

    vector<Vec4d> line1_lin2_intersection_lines = {
        {
            static_cast<double>(line1_points[0].x),  static_cast<double>(line1_points[0].y),
            static_cast<double>(line1_points[1].x - line1_points[0].x),  static_cast<double>(line1_points[1].y - line1_points[0].y)
        },
        {
            static_cast<double>(line2_points[0].x),  static_cast<double>(line2_points[0].y),
            static_cast<double>(line2_points[1].x - line2_points[0].x),  static_cast<double>(line2_points[1].y - line2_points[0].y)
        }
    };



    return ComputeOptimalIntersection(line1_lin2_intersection_lines);
}

Mat VisualPositioner::ComputeLeftCamerIntersectionShow(Mat &image)
{
    Mat image_shown;
    cvtColor(image, image_shown, CV_GRAY2BGR);

    vector<Point> line1_points;
    for (const auto& roi : camer1Line1ROI)
    {


        Point border_point = SearchPoint(image, roi,camer1line1Direction,camer1line1Threshold);
        if (border_point != Point(0, 0))
        {
            line1_points.push_back(border_point);
        }
    }

    line(image_shown, line1_points[0], line1_points[1], Scalar(0, 0, 255),8);

    vector<Point> line2_points;
    for (const auto& roi : camer1Line2ROI)
    {


        Point border_point = SearchPoint(image, roi,camer1line2Direction,camer1line2Threshold);
        if (border_point != Point(0, 0))
        {
            line2_points.push_back(border_point);
        }
    }

    line(image_shown, line2_points[0], line2_points[1], Scalar(0, 0, 255),8);





    return image_shown;
}

Mat VisualPositioner::ComputeRightCamerIntersectionShow(Mat &image)
{
    Mat image_shown;
    cvtColor(image, image_shown, CV_GRAY2BGR);

    vector<Point> line1_points;
    for (const auto& roi : camer2Line1ROI)
    {


        Point border_point = SearchPoint(image, roi,camer2line1Direction,camer2line1Threshold);
        if (border_point != Point(0, 0))
        {
            line1_points.push_back(border_point);
        }
    }

    line(image_shown, line1_points[0], line1_points[1], Scalar(0, 0, 255),8);

    vector<Point> line2_points;
    for (const auto& roi : camer2Line2ROI)
    {


        Point border_point = SearchPoint(image, roi,camer2line2Direction,camer2line2Threshold);
        if (border_point != Point(0, 0))
        {
            line2_points.push_back(border_point);
        }
    }

    line(image_shown, line2_points[0], line2_points[1], Scalar(0, 0, 255),8);



    return image_shown;
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
        Point2d cross_center = Point2d();
        if(product_image_entry.first == LeftCamera)
        {
            cross_center = ComputeLeftCamerIntersection(product_image);
        }
        else
        {
            cross_center = ComputeRightCamerIntersection(product_image);
        }

        if (cross_center == Point2d())
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
        Point2d cross_center = Point2d();
        if(product_image_entry.first == LeftCamera)
        {
            cross_center = ComputeLeftCamerIntersection(product_image);
            circle(product_image,cross_center,10,Scalar(0,0,255),4);

        }
        else
        {
            cross_center = ComputeRightCamerIntersection(product_image);
            circle(product_image,cross_center,10,Scalar(0,0,255),4);

        }

        if (cross_center == Point2d())
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
