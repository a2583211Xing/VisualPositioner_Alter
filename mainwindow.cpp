#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    bool operation_succeeded = visual_positioner.loadSettings("F:/code/VisualPositioner_Alter/settings.json");
    qDebug()<<"loadSettings:"<<operation_succeeded;





}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pushButton_calibration_registerCamera_clicked()
{
    std::string image_path = "C:/Users/Administrator.XMOIW06RV4LYV5H/Desktop/CCD定位数据2018_11_19/标定1/正/";
    std::string filename = "2.bmp";
    cv::Mat image = cv::imread(image_path + filename, cv::IMREAD_GRAYSCALE);

    bool operation_succeeded = visual_positioner.registerCamera(Camera::LeftCamera, image);

    qDebug()<<"registerLeftCamera: "<<operation_succeeded;

    filename = "1.bmp";
    image = cv::imread(image_path + filename, cv::IMREAD_GRAYSCALE);
    operation_succeeded = visual_positioner.registerCamera(Camera::RightCamera, image);
    qDebug()<<"registerRightCamera: "<<operation_succeeded;
}

void MainWindow::on_pushButton_calibration_rotationCenter_clicked()
{
    std::vector<std::string> rotation_image_paths = {
            "C:/Users/Administrator.XMOIW06RV4LYV5H/Desktop/CCD定位数据2018_11_19/标定1/偏左/",
            "C:/Users/Administrator.XMOIW06RV4LYV5H/Desktop/CCD定位数据2018_11_19/标定1/偏右/"
        };

    std::string filename = "2.bmp";
    std::vector<cv::Mat> rotation_images;
    for (const auto& img_path : rotation_image_paths)
    {
        cv::Mat rotation_image = cv::imread(img_path + filename, cv::IMREAD_GRAYSCALE);
        rotation_images.push_back(rotation_image);
    }

    /* 准备旋转图像序列 */
    std::map<Camera, std::vector<cv::Mat>> image_sequences;
    image_sequences.insert({ Camera::LeftCamera,  rotation_images });

    /* 同上，填充右相机旋转图像序列 */
    filename = "1.bmp";
    rotation_images.clear();
    for (const auto& img_path : rotation_image_paths)
    {
        cv::Mat rotation_image = cv::imread(img_path + filename, cv::IMREAD_GRAYSCALE);
        rotation_images.push_back(rotation_image);
    }
    image_sequences.insert({ Camera::RightCamera,  rotation_images });

    bool operation_succeeded = visual_positioner.calibrateRotationCenter(image_sequences);
    qDebug()<<"calibrateRotationCenter: "<<operation_succeeded;

}

void MainWindow::on_pushButton_calibration_translationDirection_clicked()
{
    std::vector<std::string> translation_image_paths = {
        "C:/Users/Administrator.XMOIW06RV4LYV5H/Desktop/CCD定位数据2018_11_19/标定1/后移/",
        "C:/Users/Administrator.XMOIW06RV4LYV5H/Desktop/CCD定位数据2018_11_19/标定1/前移/"
    };
    std::string filename = "2.bmp";
    std::vector<cv::Mat> translation_images;
    for (const auto& img_path : translation_image_paths)
    {
        cv::Mat translation_image = cv::imread(img_path + filename, cv::IMREAD_GRAYSCALE);
        translation_images.push_back(translation_image);
    }

    /* 准备平移图像序列 */
    std::map<Camera, std::vector<cv::Mat>> image_sequences_y;
    image_sequences_y.insert({ Camera::LeftCamera,  translation_images });

    /* 同上，填充右相机平移图像序列和角点平移轨迹 */
    filename = "1.bmp";
    translation_images.clear();
    for (const auto& img_path : translation_image_paths)
    {
        cv::Mat translation_image = cv::imread(img_path + filename, cv::IMREAD_GRAYSCALE);
        translation_images.push_back(translation_image);
    }

    image_sequences_y.insert({ Camera::RightCamera,  translation_images });


    translation_image_paths = {
        "C:/Users/Administrator.XMOIW06RV4LYV5H/Desktop/CCD定位数据2018_11_19/标定1/右移/",
        "C:/Users/Administrator.XMOIW06RV4LYV5H/Desktop/CCD定位数据2018_11_19/标定1/左移/"
    };

    filename = "2.bmp";
    translation_images.clear();
    for (const auto& img_path : translation_image_paths)
    {
        cv::Mat translation_image = cv::imread(img_path + filename, cv::IMREAD_GRAYSCALE);
        translation_images.push_back(translation_image);
    }

    std::map<Camera, std::vector<cv::Mat>> image_sequences_x;
    image_sequences_x.insert({ Camera::LeftCamera,  translation_images });

    filename = "1.bmp";
    translation_images.clear();
    for (const auto& img_path : translation_image_paths)
    {
        cv::Mat translation_image = cv::imread(img_path + filename, cv::IMREAD_GRAYSCALE);
        translation_images.push_back(translation_image);
    }

    image_sequences_x.insert({ Camera::RightCamera,  translation_images });

    bool operation_succeeded = visual_positioner.calibrateTranslationDirection(image_sequences_x, image_sequences_y);
    qDebug()<<"calibrateTranslationDirection: "<<operation_succeeded;
}
