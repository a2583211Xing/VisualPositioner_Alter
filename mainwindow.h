#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <VisualPositioner/VisualPositioner.h>
#include <opencv2/opencv.hpp>
#include <QHeaderView>
#include <camer_hk.h>
#include <QSettings>
#include <thread>
#include <LTDMC.h>
#include <QMessageBox>
#include <log4cxx/xLog.h>
#include <QCursor>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void drawRectSlot1(cv::Rect);
    void on_pushButton_calibration_registerCamera_clicked();
    void on_pushButton_calibration_rotationCenter_clicked();
    void on_pushButton_calibration_translationDirection_clicked();
    void on_comboBox_line1_direction_currentIndexChanged(int index);
    void on_comboBox_line2_direction_currentIndexChanged(int index);
    void on_comboBox_line1_direction_2_currentIndexChanged(int index);
    void on_comboBox_line2_direction_2_currentIndexChanged(int index);
    void on_spinBox_line1_threshold_2_valueChanged(int arg1);
    void on_spinBox_line2_threshold_2_valueChanged(int arg1);
    void on_spinBox_line1_threshold_valueChanged(int arg1);
    void on_spinBox_line2_threshold_valueChanged(int arg1);
    void on_pushButton_test_clicked();
    void on_pushButton_line1_roi1_clicked();
    void on_pushButton_line1_roi2_clicked();
    void on_pushButton_line2_roi1_clicked();
    void on_pushButton_line2_roi2_clicked();
    void on_pushButton_line1_roi1_2_clicked();
    void on_pushButton_line1_roi2_2_clicked();
    void on_pushButton_line2_roi1_2_clicked();
    void on_pushButton_line2_roi2_2_clicked();
    void on_pushButton_StandardPosition_clicked();
    void on_pushButton_camer1_Test_clicked();
    void on_pushButton_camer2_Test_clicked();
    void on_pushButton_calibration_rotationCenter_curren_clicked();
    void on_pushButton_save_Json_clicked();
    void on_pushButton_save_setting_clicked();
    void on_pushButton_calibration_rotationCenter_offset_clicked();
    void on_pushButton_calibration_translationX_curren_clicked();
    void on_pushButton_calibration_translationX_offset_clicked();
    void on_pushButton_calibration_translationY_curren_2_clicked();
    void on_pushButton_calibration_translationY_offset_2_clicked();
    void on_pushButton_run_clicked();

private:
    Ui::MainWindow *ui;

signals:
    void camer1ImageSignal(cv::Mat );
    void camer2ImageSignal(cv::Mat );

private slots:
    void camer1ImageSlot(cv::Mat );
    void camer2ImageSlot(cv::Mat );

private://算法
    VisualPositioner visual_positioner;
    void initVisualPositioner();

private://图像变量
    cv::Mat imageCamer1,imageCamer2;
    std::vector<cv::Mat> imageRotationList;
    std::vector<cv::Mat> imageTranslationXList;
    std::vector<cv::Mat> imageTranslationYList;

private://相机
    Camer_HK *camer1;
    Camer_HK *camer2;
    QString camer1IP,camer2IP;
    int camer1Ext,camer2Ext;
    bool isCamerContinue = false;
    std::thread camer1ContinueThread;
    std::thread camer2ContinueThread;

    void initCamer();
    void camer1ContinueThreadSlot();
    void camer2ContinueThreadSlot();


private://控制卡
    int axisCount;
    double  factor;
    void initMotionControl();
    bool isAllAxisStop();

    std::thread currenControlStatusThread;
    std::thread controlMoveHome;
    std::thread controlInIO;
    std::thread moveLoadProductThread;
    std::thread movePlatformThread;
    std::thread moveDetectionThread;
    std::thread moveBlankingThread;
    std::thread inProductThread;
    std::thread outProductThread;


    void currenControlStatusThreadSlot();
    void controlMoveHomeSlot();
    void controlInIOSlot();
    void moveLoadProductThreadSlot();       //上料位置
    void movePlatformThreadSlot();          //下台位置
    void moveDetectionThreadSlot();         //定位位置
    void moveBlankingThreadSlot();          //下料位置
    void inProductThreadSlot();             //上料
    void outProductThreadSlot();            //下料
private: //位置信息
    int loadProduct_Z, loadProduct_T; //上料
    int platform_Z,platform_T,platform_X,platform_Y,platform_A; //传送
    int detection_X,detection_Y,detection_A;
    int blanking_Z,blanking_T;

private slots:
    void  currenAxisPointSlot(QString,QString,QString,QString,QString);
    void on_pushButton_control_moveZero_clicked();
    void on_pushButton_moveP_negative_axisX_pressed();
    void on_pushButton_moveP_negative_axisX_released();
    void on_pushButton_moveP_positive_axisX_pressed();
    void on_pushButton_moveP_positive_axisX_released();
    void on_pushButton_moveP_negative_axisY_pressed();
    void on_pushButton_moveP_negative_axisY_released();
    void on_pushButton_moveP_positive_axisY_pressed();
    void on_pushButton_moveP_positive_axisY_released();
    void on_pushButton_moveP_negative_axisZ_pressed();
    void on_pushButton_moveP_negative_axisZ_released();
    void on_pushButton_moveP_positive_axisZ_pressed();
    void on_pushButton_moveP_positive_axisZ_released();
    void on_pushButton_moveP_negative_axisT_pressed();
    void on_pushButton_moveP_negative_axisT_released();
    void on_pushButton_moveP_positive_axisT_pressed();
    void on_pushButton_moveP_positive_axisT_released();
    void on_pushButton_moveP_negative_axisA_pressed();
    void on_pushButton_moveP_negative_axisA_released();
    void on_pushButton_moveP_positive_axisA_pressed();
    void on_pushButton_moveP_positive_axisA_released();
    void on_pushButton_control_record_load_clicked();
    void on_pushButton_control_move_load_clicked();
    void on_pushButton_control_record_platform_clicked();
    void on_pushButton_control_move_platform_clicked();
    void on_pushButton_control_record_detection_clicked();
    void on_pushButton_control_move_detection_clicked();
    void on_pushButton_control_record_blanking_clicked();
    void on_pushButton_control_move_blanking_clicked();



    void on_pushButton_control_enable_toggled(bool checked);

    void on_checkBox_clicked(bool checked);

    void on_pushButton_move_axisX_clicked();


    void on_pushButton_control_proudctIO_toggled(bool checked);

    void on_pushButton_control_platformIO_toggled(bool checked);

    void on_pushButton_inProduct_clicked();

    void on_pushButton_outProduct_clicked();


    void on_pushButton_set_camer_ext_clicked();

    void on_pushButton_set_camer_ext2_clicked();

    void on_pushButton_move_axisY_clicked();

    void on_pushButton_camerContinue_toggled(bool checked);


    void on_pushButton_savePicture_clicked();

signals:
    void currenAxisPoint(QString,QString,QString,QString,QString);




private://配置文件
    QSettings *setting;

    void initSetting();





private://标志
    int drawRectFlag = -1;
    log4cxx::LoggerPtr  logger;
    void    InitLog(QString logname, QString sys_version);

private:
    int tempIndex = 1;
};



#endif // MAINWINDOW_H
