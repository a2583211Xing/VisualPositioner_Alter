#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    camer1 = NULL;
    camer2 = NULL;


    InitLog("MainWindow","1.0.0");
    this->initSetting();
    this->initVisualPositioner();
    this->initCamer();
    this->initMotionControl();


    connect(ui->scrollArea_camer1,SIGNAL(drawRectSignal(cv::Rect)),this,SLOT(drawRectSlot1(cv::Rect)));
    connect(ui->scrollArea_camer2,SIGNAL(drawRectSignal(cv::Rect)),this,SLOT(drawRectSlot1(cv::Rect)));
    connect(this,SIGNAL(camer1ImageSignal(cv::Mat)),this,SLOT(camer1ImageSlot(cv::Mat)));
    connect(this,SIGNAL(camer2ImageSignal(cv::Mat)),this,SLOT(camer2ImageSlot(cv::Mat)));

    bool checked = false;
    ui->pushButton_control_record_blanking->setEnabled(checked);
    ui->pushButton_control_record_load->setEnabled(checked);
    ui->pushButton_control_record_platform->setEnabled(checked);
    ui->pushButton_control_record_detection->setEnabled(checked);


}

MainWindow::~MainWindow()
{
    for(int i=0; i<axisCount ; i++)
        dmc_set_sevon_enable(0,i,0);
    dmc_board_close();
    delete ui;
}

void MainWindow::drawRectSlot1(cv::Rect rect)
{

    switch (drawRectFlag) {
    case 0:
        visual_positioner.camer1Line1ROI.at(0) = rect;
        ui->label_line1_roi1->setText(QString("Rect:%1,%2,%3,%4").arg(visual_positioner.camer1Line1ROI.at(0).x)
                                                                 .arg(visual_positioner.camer1Line1ROI.at(0).y)
                                                                 .arg(visual_positioner.camer1Line1ROI.at(0).width)
                                                                 .arg(visual_positioner.camer1Line1ROI.at(0).height));
        break;
    case 1:
        visual_positioner.camer1Line1ROI.at(1) = rect;
        ui->label_line1_roi2->setText(QString("Rect:%1,%2,%3,%4").arg(visual_positioner.camer1Line1ROI.at(1).x)
                                                                 .arg(visual_positioner.camer1Line1ROI.at(1).y)
                                                                 .arg(visual_positioner.camer1Line1ROI.at(1).width)
                                                                 .arg(visual_positioner.camer1Line1ROI.at(1).height));

        break;
    case 2:
        visual_positioner.camer1Line2ROI.at(0) = rect;
        ui->label_line2_roi1->setText(QString("Rect:%1,%2,%3,%4").arg(visual_positioner.camer1Line2ROI.at(0).x)
                                                                 .arg(visual_positioner.camer1Line2ROI.at(0).y)
                                                                 .arg(visual_positioner.camer1Line2ROI.at(0).width)
                                                                 .arg(visual_positioner.camer1Line2ROI.at(0).height));

        break;
    case 3:
        visual_positioner.camer1Line2ROI.at(1) = rect;
        ui->label_line2_roi2->setText(QString("Rect:%1,%2,%3,%4").arg(visual_positioner.camer1Line2ROI.at(1).x)
                                                                 .arg(visual_positioner.camer1Line2ROI.at(1).y)
                                                                 .arg(visual_positioner.camer1Line2ROI.at(1).width)
                                                                 .arg(visual_positioner.camer1Line2ROI.at(1).height));

        break;
    case 4:
        visual_positioner.camer2Line1ROI.at(0) = rect;
        ui->label_line1_roi1_2->setText(QString("Rect:%1,%2,%3,%4").arg(visual_positioner.camer2Line1ROI.at(0).x)
                                                                   .arg(visual_positioner.camer2Line1ROI.at(0).y)
                                                                   .arg(visual_positioner.camer2Line1ROI.at(0).width)
                                                                   .arg(visual_positioner.camer2Line1ROI.at(0).height));

        break;
    case 5:
        visual_positioner.camer2Line1ROI.at(1) = rect;
        ui->label_line1_roi2_2->setText(QString("Rect:%1,%2,%3,%4").arg(visual_positioner.camer2Line1ROI.at(1).x)
                                                                   .arg(visual_positioner.camer2Line1ROI.at(1).y)
                                                                   .arg(visual_positioner.camer2Line1ROI.at(1).width)
                                                                   .arg(visual_positioner.camer2Line1ROI.at(1).height));

        break;
    case 6:
        visual_positioner.camer2Line2ROI.at(0) = rect;
        ui->label_line2_roi1_2->setText(QString("Rect:%1,%2,%3,%4").arg(visual_positioner.camer2Line2ROI.at(0).x)
                                                                   .arg(visual_positioner.camer2Line2ROI.at(0).y)
                                                                   .arg(visual_positioner.camer2Line2ROI.at(0).width)
                                                                   .arg(visual_positioner.camer2Line2ROI.at(0).height));


        break;
    case 7:
        visual_positioner.camer2Line2ROI.at(1) = rect;
        ui->label_line2_roi2_2->setText(QString("Rect:%1,%2,%3,%4").arg(visual_positioner.camer2Line2ROI.at(1).x)
                                                                   .arg(visual_positioner.camer2Line2ROI.at(1).y)
                                                                   .arg(visual_positioner.camer2Line2ROI.at(1).width)
                                                                   .arg(visual_positioner.camer2Line2ROI.at(1).height));


        break;
    default:
        break;
    }

    drawRectFlag =  -1;


   ui->textBrowser->append(QString("Rect:%1,%2,%3,%4").arg(rect.x).arg(rect.y).arg(rect.width).arg(rect.height));
}



void MainWindow::on_pushButton_calibration_registerCamera_clicked()
{
    bool operation_succeeded;
    if(imageCamer1.data)
    {

        operation_succeeded = visual_positioner.registerCamera(Camera::LeftCamera, imageCamer1);
        ui->textBrowser->append("registerLeftCamera: " + operation_succeeded?"True":"False");

    }

    if(imageCamer2.data)
    {
        operation_succeeded = visual_positioner.registerCamera(Camera::RightCamera, imageCamer2);
        ui->textBrowser->append("registerRightCamera: " + operation_succeeded?"True":"False") ;
    }


}

void MainWindow::on_pushButton_calibration_rotationCenter_clicked()
{
//    for(int i=0;i<imageRotationList.size();i++)
//    {
//        QString fileName = QString("./data/%1.bmp").arg(i);
//        cv::imwrite(fileName.toStdString(),imageRotationList.at(i));
//    }
    for(int i=0 ; i< imageRotationList.size() ; i++)
    {
        if(!imageRotationList.at(i).data)
        {
            ui->textBrowser->append(QString("imageRotationList lose:%1").arg(i));
            return ;
        }
    }
    std::vector<cv::Mat> rotation_images;
//    std::vector<std::string> rotation_image_paths = {
//            "C:/Users/Administrator.XMOIW06RV4LYV5H/Desktop/CCD定位数据2018_11_19/标定1/偏左/",
//            "C:/Users/Administrator.XMOIW06RV4LYV5H/Desktop/CCD定位数据2018_11_19/标定1/偏右/"
//        };

//    std::string filename = "2.bmp";

//    for (const auto& img_path : rotation_image_paths)
//    {
//        cv::Mat rotation_image = cv::imread(img_path + filename, cv::IMREAD_GRAYSCALE);
//        rotation_images.push_back(rotation_image);
//    }
    rotation_images.push_back(imageRotationList.at(0));
    rotation_images.push_back(imageRotationList.at(2));
    /* 准备旋转图像序列 */
    std::map<Camera, std::vector<cv::Mat>> image_sequences;
    image_sequences.insert({ Camera::LeftCamera,  rotation_images });

    /* 同上，填充右相机旋转图像序列 */
//    filename = "1.bmp";
    rotation_images.clear();
//    for (const auto& img_path : rotation_image_paths)
//    {
//        cv::Mat rotation_image = cv::imread(img_path + filename, cv::IMREAD_GRAYSCALE);
//        rotation_images.push_back(rotation_image);
//    }

    rotation_images.push_back(imageRotationList.at(1));
    rotation_images.push_back(imageRotationList.at(3));
    image_sequences.insert({ Camera::RightCamera,  rotation_images });

    bool operation_succeeded = visual_positioner.calibrateRotationCenter(image_sequences);
    ui->textBrowser->append("calibrateRotationCenter: " + operation_succeeded?"True":"False");

}

void MainWindow::on_pushButton_calibration_translationDirection_clicked()
{
    for(int i=0 ; i<imageTranslationXList.size(); i++)
    {
        if(!imageTranslationXList.at(i).data)
        {
            ui->textBrowser->append(QString("imageTranslationXList lose : %1").arg(i));
            return ;
        }
    }

    for(int i=0 ; i<imageTranslationYList.size(); i++)
    {
        if(!imageTranslationYList.at(i).data)
        {
            ui->textBrowser->append(QString("imageTranslationYList lose : %1").arg(i));
            return ;
        }
    }

    std::vector<cv::Mat> translation_images;
//    std::vector<std::string> translation_image_paths = {
//        "C:/Users/Administrator.XMOIW06RV4LYV5H/Desktop/CCD定位数据2018_11_19/标定1/后移/",
//        "C:/Users/Administrator.XMOIW06RV4LYV5H/Desktop/CCD定位数据2018_11_19/标定1/前移/"
//    };
//    std::string filename = "2.bmp";
//
//    for (const auto& img_path : translation_image_paths)
//    {
//        cv::Mat translation_image = cv::imread(img_path + filename, cv::IMREAD_GRAYSCALE);
//        translation_images.push_back(translation_image);
//    }

    translation_images.push_back(imageTranslationYList.at(0));
    translation_images.push_back(imageTranslationYList.at(2));

    /* 准备平移图像序列 */
    std::map<Camera, std::vector<cv::Mat>> image_sequences_y;
    image_sequences_y.insert({ Camera::LeftCamera,  translation_images });

    /* 同上，填充右相机平移图像序列和角点平移轨迹 */
//    filename = "1.bmp";
    translation_images.clear();
//    for (const auto& img_path : translation_image_paths)
//    {
//        cv::Mat translation_image = cv::imread(img_path + filename, cv::IMREAD_GRAYSCALE);
//        translation_images.push_back(translation_image);
//    }

    translation_images.push_back(imageTranslationYList.at(1));
    translation_images.push_back(imageTranslationYList.at(3));

    image_sequences_y.insert({ Camera::RightCamera,  translation_images });


//    translation_image_paths = {
//        "C:/Users/Administrator.XMOIW06RV4LYV5H/Desktop/CCD定位数据2018_11_19/标定1/右移/",
//        "C:/Users/Administrator.XMOIW06RV4LYV5H/Desktop/CCD定位数据2018_11_19/标定1/左移/"
//    };

//    filename = "2.bmp";
    translation_images.clear();
//    for (const auto& img_path : translation_image_paths)
//    {
//        cv::Mat translation_image = cv::imread(img_path + filename, cv::IMREAD_GRAYSCALE);
//        translation_images.push_back(translation_image);
//    }

    translation_images.push_back(imageTranslationXList.at(0));
    translation_images.push_back(imageTranslationXList.at(2));

    std::map<Camera, std::vector<cv::Mat>> image_sequences_x;
    image_sequences_x.insert({ Camera::LeftCamera,  translation_images });

//    filename = "1.bmp";
    translation_images.clear();
//    for (const auto& img_path : translation_image_paths)
//    {
//        cv::Mat translation_image = cv::imread(img_path + filename, cv::IMREAD_GRAYSCALE);
//        translation_images.push_back(translation_image);
//    }
    translation_images.push_back(imageTranslationXList.at(1));
    translation_images.push_back(imageTranslationXList.at(3));

    image_sequences_x.insert({ Camera::RightCamera,  translation_images });

    bool operation_succeeded = visual_positioner.calibrateTranslationDirection(image_sequences_x, image_sequences_y);
    ui->textBrowser->append("calibrateTranslationDirection: " + operation_succeeded?"True":"False");
}


void MainWindow::on_comboBox_line1_direction_currentIndexChanged(int index)
{
    visual_positioner.camer1line1Direction = index;
}



void MainWindow::on_comboBox_line2_direction_currentIndexChanged(int index)
{
    visual_positioner.camer1line2Direction = index;
}

void MainWindow::on_comboBox_line1_direction_2_currentIndexChanged(int index)
{
    visual_positioner.camer2line1Direction = index;
}

void MainWindow::on_comboBox_line2_direction_2_currentIndexChanged(int index)
{
    visual_positioner.camer2line2Direction = index;
}

void MainWindow::initVisualPositioner()
{
    Log_Debug(logger,"initVisualPositioner Start");
    bool operation_succeeded = visual_positioner.loadSettings("./Setting/settings.json");


    ui->comboBox_line1_direction->setCurrentIndex(visual_positioner.camer1line1Direction);
    ui->comboBox_line2_direction->setCurrentIndex(visual_positioner.camer1line2Direction);

    ui->comboBox_line1_direction_2->setCurrentIndex(visual_positioner.camer2line1Direction);
    ui->comboBox_line2_direction_2->setCurrentIndex(visual_positioner.camer2line2Direction);

    ui->spinBox_line1_threshold->setValue(visual_positioner.camer1line1Threshold);
    ui->spinBox_line2_threshold->setValue(visual_positioner.camer1line2Threshold);

    ui->spinBox_line1_threshold_2->setValue(visual_positioner.camer2line1Threshold);
    ui->spinBox_line2_threshold_2->setValue(visual_positioner.camer2line2Threshold);

    ui->label_line1_roi1->setText(QString("Rect:%1,%2,%3,%4").arg(visual_positioner.camer1Line1ROI.at(0).x)
                                                             .arg(visual_positioner.camer1Line1ROI.at(0).y)
                                                             .arg(visual_positioner.camer1Line1ROI.at(0).width)
                                                             .arg(visual_positioner.camer1Line1ROI.at(0).height));

    ui->label_line1_roi2->setText(QString("Rect:%1,%2,%3,%4").arg(visual_positioner.camer1Line1ROI.at(1).x)
                                                             .arg(visual_positioner.camer1Line1ROI.at(1).y)
                                                             .arg(visual_positioner.camer1Line1ROI.at(1).width)
                                                             .arg(visual_positioner.camer1Line1ROI.at(1).height));

    ui->label_line2_roi1->setText(QString("Rect:%1,%2,%3,%4").arg(visual_positioner.camer1Line2ROI.at(0).x)
                                                             .arg(visual_positioner.camer1Line2ROI.at(0).y)
                                                             .arg(visual_positioner.camer1Line2ROI.at(0).width)
                                                             .arg(visual_positioner.camer1Line2ROI.at(0).height));

    ui->label_line2_roi2->setText(QString("Rect:%1,%2,%3,%4").arg(visual_positioner.camer1Line2ROI.at(1).x)
                                                             .arg(visual_positioner.camer1Line2ROI.at(1).y)
                                                             .arg(visual_positioner.camer1Line2ROI.at(1).width)
                                                             .arg(visual_positioner.camer1Line2ROI.at(1).height));

    ui->label_line1_roi1_2->setText(QString("Rect:%1,%2,%3,%4").arg(visual_positioner.camer2Line1ROI.at(0).x)
                                                               .arg(visual_positioner.camer2Line1ROI.at(0).y)
                                                               .arg(visual_positioner.camer2Line1ROI.at(0).width)
                                                               .arg(visual_positioner.camer2Line1ROI.at(0).height));

    ui->label_line1_roi2_2->setText(QString("Rect:%1,%2,%3,%4").arg(visual_positioner.camer2Line1ROI.at(1).x)
                                                               .arg(visual_positioner.camer2Line1ROI.at(1).y)
                                                               .arg(visual_positioner.camer2Line1ROI.at(1).width)
                                                               .arg(visual_positioner.camer2Line1ROI.at(1).height));

    ui->label_line2_roi1_2->setText(QString("Rect:%1,%2,%3,%4").arg(visual_positioner.camer2Line2ROI.at(0).x)
                                                               .arg(visual_positioner.camer2Line2ROI.at(0).y)
                                                               .arg(visual_positioner.camer2Line2ROI.at(0).width)
                                                               .arg(visual_positioner.camer2Line2ROI.at(0).height));

    ui->label_line2_roi2_2->setText(QString("Rect:%1,%2,%3,%4").arg(visual_positioner.camer2Line2ROI.at(1).x)
                                                               .arg(visual_positioner.camer2Line2ROI.at(1).y)
                                                               .arg(visual_positioner.camer2Line2ROI.at(1).width)
                                                               .arg(visual_positioner.camer2Line2ROI.at(1).height));

    for(int i=0; i<4 ; i++)
    {
        imageRotationList.push_back(cv::Mat());
        imageTranslationXList.push_back(cv::Mat());
        imageTranslationYList.push_back(cv::Mat());

    }

    Log_Debug(logger,"initVisualPositioner End");
}

void MainWindow::initCamer()
{
    Log_Debug(logger,"initCamer Start");
    if(camer1 != NULL)
    {
        delete camer1;
        camer1 = NULL;
    }
    camer1 = new Camer_HK();
    if(camer1->OpenCamera(camer1IP))
    {
        camer1->SetExpTime(camer1Ext);
        ui->textBrowser->append( "Open Camer1 : True");
    }
    else
    {
        ui->textBrowser->append( "Open Camer1 : False");
        delete camer1;
        camer1 = NULL;
    }

    if(camer2 != NULL)
    {
        delete camer2;
        camer2 = NULL;
    }

    camer2 = new Camer_HK();
    if(camer2->OpenCamera(camer2IP))
    {
        camer2->SetExpTime(camer2Ext);
        ui->textBrowser->append( "Open Camer2 : True");
    }
    else
    {
        ui->textBrowser->append( "Open Camer2 : False");
        delete camer2;
        camer2 = NULL;
    }

    Log_Debug(logger,"initCamer End");
}

void MainWindow::camer1ContinueThreadSlot()
{
    cv::Mat img;
    while (isCamerContinue) {
        img = camer1->GetOneFrame();
        ui->scrollArea_camer1->setImage(img);
        Sleep(200);

    }
}

void MainWindow::camer2ContinueThreadSlot()
{
    cv::Mat img;
    while (isCamerContinue) {
        img = camer2->GetOneFrame();
        ui->scrollArea_camer2->setImage(img);

        Sleep(100);

    }
}

void MainWindow::initMotionControl()
{
    Log_Debug(logger,"initMotionControl Start");

    axisCount = 5;
    factor = (double)5 / (double)131072;
    if( dmc_board_init() <= 0 )
    {
        QMessageBox::information(this,"Error","Initialization control business card printing failed!");
        return ;
    }


    currenControlStatusThread = std::thread(&MainWindow::currenControlStatusThreadSlot,this);
    currenControlStatusThread.detach();

    controlInIO = std::thread(&MainWindow::controlInIOSlot,this);
    controlInIO.detach();

    connect(this,SIGNAL(currenAxisPoint(QString,QString,QString,QString,QString)),this,SLOT(currenAxisPointSlot(QString,QString,QString,QString,QString)));

    dmc_write_outbit(0,7,0);
    Log_Debug(logger,"initMotionControl End");

}

bool MainWindow::isAllAxisStop()
{
    if( dmc_check_done(0,0) && dmc_check_done(0,1) && dmc_check_done(0,2) && dmc_check_done(0,3) && dmc_check_done(0,4))
        return true;
    else
        return false;
}

void MainWindow::currenControlStatusThreadSlot()
{

    double x,y,z,t,a;
    while (true) {
        x = dmc_get_position(0,0);
        y = dmc_get_position(0,1);
        z = dmc_get_position(0,2);
        t = dmc_get_position(0,3);
        a = dmc_get_position(0,4);

        emit currenAxisPoint( QString::number(x*factor,'f', 3),
                              QString::number(y*factor,'f', 3),
                              QString::number(z*factor,'f', 3),
                              QString::number(t*factor,'f', 3),
                              QString::number(a*factor,'f', 3));

        Sleep(200);

    }
}

void MainWindow::controlMoveHomeSlot()
{
    dmc_set_homemode( 0 , 2 , 1 , 1 , 0 , 0 );
    dmc_home_move( 0 , 2 );
    while (true) {
        if(dmc_check_done(0,2))
        {
            break;
        }
        Sleep(100);

    }

    dmc_set_homemode( 0 , 0 , 0 , 1 , 0 , 0 );
    dmc_home_move( 0 , 0 );

    dmc_set_homemode( 0 , 1 , 0 , 1 , 0 , 0 );
    dmc_home_move( 0 , 1 );



    dmc_set_homemode( 0 , 3 , 0 , 1 , 4 , 0 );
    dmc_home_move( 0 , 3 );

    while (true) {
        if(dmc_check_done(0,3))
        {
            break;
        }
        Sleep(100);

    }

     if( (dmc_axis_io_status(0,3)&0B100) == 0B100 )
     {
         dmc_set_homemode( 0 , 3 , 0 , 1 , 4 , 0 );
         dmc_home_move( 0 , 3 );
         while (true) {
             if(dmc_check_done(0,3))
             {
                 break;
             }
             Sleep(100);

         }

     }

     while (true) {
         if(isAllAxisStop())
         {
             break;
         }
         Sleep(100);

     }



     for(int i=0; i<axisCount; i++)
     {
         dmc_set_position(0,i,0);
         dmc_set_encoder(0,i,0);
     }
}

void MainWindow::controlInIOSlot()
{
    //0：停止   1：复位   2：运行    3：急停
    //1 0 0 1
    while (true) {
        if( dmc_read_inbit(0,1) == 0)
        {
            for(int i=0; i<6; i++)
            {
                dmc_write_erc_pin( 0 , i , 0 ) ;

            }
            while (true) {
                if( dmc_read_inbit(0,1) == 1 )
                    break;
                Sleep(200);
            }
            for(int i=0; i<6; i++)
            {
                dmc_write_erc_pin( 0 , i , 1 ) ;

            }
            ui->textBrowser->append("RST");
            ui->textBrowser->moveCursor(QTextCursor::End);


        }
        if( dmc_read_inbit(0,2) == 0)
        {
            while (true) {
                if( dmc_read_inbit(0,2) == 1 )
                {
                    break;
                }
                Sleep(200);
            }
            ui->textBrowser->append("Run");
            ui->textBrowser->moveCursor(QTextCursor::End);
        }
        if( dmc_read_inbit(0,3) == 1)
        {
            ui->textBrowser->append("Stop");
            ui->textBrowser->moveCursor(QTextCursor::End);
            dmc_stop( 0 , 0 , 1 );
            dmc_stop( 0 , 1 , 1 );
            dmc_stop( 0 , 2 , 1 );
            dmc_stop( 0 , 3 , 1 );
            dmc_stop( 0 , 4 , 1 );
            dmc_stop( 0 , 5 , 1 );
        }
        Sleep(200);
    }
}

void MainWindow::moveLoadProductThreadSlot()
{
    dmc_pmove(0,2,0,1);
    while (true) {
        if(dmc_check_done(0,2))
        {
            break;
        }
        Sleep(100);
    }
    dmc_pmove(0,3,loadProduct_T,1);
    while (true) {
        if(dmc_check_done(0,3))
        {
            break;
        }
        Sleep(100);
    }
    dmc_pmove(0,2,loadProduct_Z,1);
    while (true) {
        if(dmc_check_done(0,2))
        {
            break;
        }
        Sleep(100);
    }
}

void MainWindow::movePlatformThreadSlot()
{
    dmc_pmove(0,2,0,1);
    while (true) {
        if(dmc_check_done(0,2))
        {
            break;
        }
        Sleep(100);
    }
    dmc_pmove(0,3,platform_T,1);
    Sleep(10);
    dmc_pmove(0,0,platform_X,1);
    Sleep(10);
    dmc_pmove(0,1,platform_Y,1);
    Sleep(10);
    dmc_pmove(0,4,platform_A,1);

    while (true) {
        if( isAllAxisStop() )
        {
            break;
        }
        Sleep(100);
    }
    dmc_pmove(0,2,platform_Z,1);
    while (true) {
        if(dmc_check_done(0,2))
        {
            break;
        }
        Sleep(100);
    }
}

void MainWindow::moveDetectionThreadSlot()
{
    dmc_pmove(0,2,0,1);
    while (true) {
        if(dmc_check_done(0,2))
        {
            break;
        }
        Sleep(100);
    }

    dmc_pmove(0,0,detection_X,1);
    Sleep(10);
    dmc_pmove(0,1,detection_Y,1);
    Sleep(10);
    dmc_pmove(0,4,detection_A,1);
    while (true) {
        if(isAllAxisStop())
        {
            break;
        }
        Sleep(100);
    }

}

void MainWindow::moveBlankingThreadSlot()
{
    dmc_pmove(0,2,0,1);
    while (true) {
        if(dmc_check_done(0,2))
        {
            break;
        }
        Sleep(100);
    }
    dmc_pmove(0,3,blanking_T,1);
    while (true) {
        if(dmc_check_done(0,3))
        {
            break;
        }
        Sleep(100);
    }

    dmc_pmove(0,2,blanking_Z,1);
    while (true) {
        if(dmc_check_done(0,2))
        {
            break;
        }
        Sleep(100);
    }


}

void MainWindow::inProductThreadSlot()
{
    this->moveLoadProductThreadSlot();
    Sleep(100);
    this->on_pushButton_control_proudctIO_toggled(true);
    Sleep(100);
    this->movePlatformThreadSlot();
    Sleep(100);
    this->on_pushButton_control_proudctIO_toggled(false);
    this->on_pushButton_control_platformIO_toggled(true);
    Sleep(100);
    this->moveDetectionThreadSlot();





}

void MainWindow::outProductThreadSlot()
{
    dmc_pmove(0,2,0,1);
    while (true) {
        if(dmc_check_done(0,2))
        {
            break;
        }
        Sleep(100);
    }
    dmc_pmove(0,1,platform_Y - detection_Y,0);
    while (true) {
        if(dmc_check_done(0,1))
        {
            break;
        }
        Sleep(100);
    }
    dmc_pmove(0,2,platform_Z,1);
    while (true) {
        if(dmc_check_done(0,2))
        {
            break;
        }
        Sleep(100);
    }
    Sleep(100);
    this->on_pushButton_control_platformIO_toggled(false);
    this->on_pushButton_control_proudctIO_toggled(true);
    Sleep(100);
    this->moveBlankingThreadSlot();
    this->on_pushButton_control_proudctIO_toggled(false);
    dmc_pmove(0,2,0,1);
    while (true) {
        if(dmc_check_done(0,2))
        {
            break;
        }
        Sleep(100);
    }

}

void MainWindow::currenAxisPointSlot(QString x, QString y, QString z, QString t, QString a)
{
    ui->lineEdit_readOnly_current_axisX->setText(x);
    ui->lineEdit_readOnly_current_axisY->setText(y);
    ui->lineEdit_readOnly_current_axisZ->setText(z);
    ui->lineEdit_readOnly_current_axisT->setText(t);
    ui->lineEdit_readOnly_current_axisA->setText(a);
}

void MainWindow::initSetting()
{
    Log_Debug(logger,"initSetting Start");
    setting = new QSettings("./Setting/setting.ini",QSettings::IniFormat);


    camer1IP  = setting->value("Camer1/CamerIP").toString();
    camer1Ext = setting->value("Camer1/CamerExt").toInt();

    ui->lineEdit_camer_ip->setText(camer1IP);
    ui->lineEdit_camer_ext->setText(QString::number(camer1Ext));

    camer2IP  = setting->value("Camer2/CamerIP").toString();
    camer2Ext = setting->value("Camer2/CamerExt").toInt();

    ui->lineEdit_camer_ip2->setText(camer2IP);
    ui->lineEdit_camer_ext2->setText(QString::number(camer2Ext));

    loadProduct_Z = setting->value("Place/loadProduct_Z").toInt();
    loadProduct_T = setting->value("Place/loadProduct_T").toInt();

    platform_Z = setting->value("Place/platform_Z").toInt();
    platform_T = setting->value("Place/platform_T").toInt();
    platform_X = setting->value("Place/platform_X").toInt();
    platform_Y = setting->value("Place/platform_Y").toInt();
    platform_A = setting->value("Place/platform_A").toInt();

    detection_X = setting->value("Place/detection_X").toInt();
    detection_Y = setting->value("Place/detection_Y").toInt();
    detection_A = setting->value("Place/detection_A").toInt();

    blanking_Z = setting->value("Place/blanking_Z").toInt();
    blanking_T = setting->value("Place/blanking_T").toInt();



    Log_Debug(logger,"initSetting End");

}

void MainWindow::InitLog(QString logname, QString sys_version)
{
    QDir dir;
    bool exist = dir.exists( "./debug_message" );
    if(!exist)
    {
        dir.mkpath( "./debug_message" );
    }

    logger = Logger::getLogger( "MainWindow" );
    helpers::Pool p;

    PatternLayoutPtr layout = new PatternLayout();
    LogString conversionPattern = LogString( Log_Layout );
    layout->setConversionPattern( conversionPattern );

    LogString LogPath = ( L"./debug_message/" + logname.toStdWString() + L".log" );

    DailyRollingFileAppenderPtr rollingfileAppender = new DailyRollingFileAppender();
    rollingfileAppender->setFile(LogPath);
    rollingfileAppender->setAppend(false);
    rollingfileAppender->setDatePattern(DatePattern);
    rollingfileAppender->setLayout(LayoutPtr(layout));
    rollingfileAppender->activateOptions(p);
    rollingfileAppender->setEncoding(L"UTF-8");
    rollingfileAppender->setName(L"rollingfileAppender");
    logger->addAppender(AppenderPtr(rollingfileAppender));
    logger->setAdditivity(false);//不继承root设置
    logger->setLevel(Level::getTrace());

    Log_Info(logger,"\n\n\n"
                    "                          start    %s   system!!                         "
                    "\n\n"
                    "************************** %s %s--%s ********************* "
                    "\n\n",
             logname.toLatin1().data(), sys_version.toLatin1().data(), __DATE__, __TIME__ );

}

void MainWindow::on_spinBox_line1_threshold_2_valueChanged(int arg1)
{
    visual_positioner.camer2line1Threshold = arg1;
}

void MainWindow::on_spinBox_line2_threshold_2_valueChanged(int arg1)
{
    visual_positioner.camer2line2Threshold = arg1;
}



void MainWindow::on_spinBox_line1_threshold_valueChanged(int arg1)
{
    visual_positioner.camer1line1Threshold = arg1;
}

void MainWindow::on_spinBox_line2_threshold_valueChanged(int arg1)
{
    visual_positioner.camer1line2Threshold = arg1;
}

void MainWindow::on_pushButton_test_clicked()
{
    if(camer1 == NULL || camer2 == NULL)
        return;
    imageCamer1 = camer1->GetOneFrame();
    imageCamer2 = camer2->GetOneFrame();

//    imageCamer1 = cv::imread("camer1.bmp",CV_8U);
//    imageCamer2 = cv::imread("camer2.bmp",CV_8U);

//    QDir dir;
//    QString fileDir = QString("./data/%1/").arg(tempIndex);

//    bool exist = dir.exists( fileDir );
//    if(!exist)
//    {
//        dir.mkpath(  fileDir );
//    }
//    QString fileName = fileDir + "Camer1.bmp";
//    cv::imwrite(fileName.toStdString(),imageCamer1);
//    fileName = fileDir + "Camer2.bmp";
//    cv::imwrite(fileName.toStdString(),imageCamer2);

    ui->scrollArea_camer1->setImage(imageCamer1);
    ui->scrollArea_camer2->setImage(imageCamer2);
    tempIndex++;
}

void MainWindow::on_pushButton_line1_roi1_clicked()
{
    drawRectFlag = 0;
    ui->scrollArea_camer1->setDraw(true);
}

void MainWindow::on_pushButton_line1_roi2_clicked()
{
    drawRectFlag = 1;
    ui->scrollArea_camer1->setDraw(true);
}

void MainWindow::on_pushButton_line2_roi1_clicked()
{
    drawRectFlag = 2;
    ui->scrollArea_camer1->setDraw(true);
}

void MainWindow::on_pushButton_line2_roi2_clicked()
{
    drawRectFlag = 3;
    ui->scrollArea_camer1->setDraw(true);
}

void MainWindow::on_pushButton_line1_roi1_2_clicked()
{
    drawRectFlag = 4;
    ui->scrollArea_camer2->setDraw(true);
}

void MainWindow::on_pushButton_line1_roi2_2_clicked()
{
    drawRectFlag = 5;
    ui->scrollArea_camer2->setDraw(true);
}

void MainWindow::on_pushButton_line2_roi1_2_clicked()
{
    drawRectFlag = 6;
    ui->scrollArea_camer2->setDraw(true);
}

void MainWindow::on_pushButton_line2_roi2_2_clicked()
{
    drawRectFlag = 7;
    ui->scrollArea_camer2->setDraw(true);
}

void MainWindow::on_pushButton_StandardPosition_clicked()
{

    if(imageCamer1.data && imageCamer2.data)
    {
        std::map<Camera, cv::Mat> standard_position_images = { { Camera::LeftCamera, imageCamer1 } };

        standard_position_images.insert({ Camera::RightCamera, imageCamer2 });
        bool operation_succeeded = visual_positioner.registerStandardPosition(standard_position_images);
        ui->textBrowser->append("registerStandardPosition:" + operation_succeeded ? "True":"False");

    }

}

void MainWindow::on_pushButton_camer1_Test_clicked()
{
    if(imageCamer1.data)
    {
        ui->scrollArea_camer1->setImage(visual_positioner.ComputeLeftCamerIntersectionShow(imageCamer1));
    }
}

void MainWindow::on_pushButton_camer2_Test_clicked()
{
    if(imageCamer2.data)
    {
        ui->scrollArea_camer2->setImage(visual_positioner.ComputeRightCamerIntersectionShow(imageCamer2));
    }
}

void MainWindow::on_pushButton_save_Json_clicked()
{
    if(visual_positioner.saveSettings("./Setting/settings.json"))
    {
        ui->textBrowser->append("SaveSettings :True");
    }
    else
    {
         ui->textBrowser->append("SaveSettings :False");
    }
}

void MainWindow::on_pushButton_save_setting_clicked()
{
    setting->setValue("Camer1/CamerIP",ui->lineEdit_camer_ip->text());
    setting->setValue("Camer1/CamerExt",ui->lineEdit_camer_ext->text().toInt());

    setting->setValue("Camer2/CamerIP",ui->lineEdit_camer_ip2->text());
    setting->setValue("Camer2/CamerExt",ui->lineEdit_camer_ext2->text().toInt());

    setting->setValue("Place/loadProduct_Z",loadProduct_Z);
    setting->setValue("Place/loadProduct_T",loadProduct_T);

    setting->setValue("Place/platform_Z",platform_Z);
    setting->setValue("Place/platform_T",platform_T);
    setting->setValue("Place/platform_X",platform_X);
    setting->setValue("Place/platform_Y",platform_Y);
    setting->setValue("Place/platform_A",platform_A);

    setting->setValue("Place/detection_X",detection_X);
    setting->setValue("Place/detection_Y",detection_Y);
    setting->setValue("Place/detection_A",detection_A);

    setting->setValue("Place/blanking_Z",blanking_Z);
    setting->setValue("Place/blanking_T",blanking_T);



}

void MainWindow::on_pushButton_calibration_rotationCenter_curren_clicked()
{
    if(camer1 != NULL)
    {
        imageRotationList.at(0) = camer1->GetOneFrame();
    }
    else
    {
        ui->textBrowser->append("Camer1 is NULL");
    }

    if(camer2 != NULL)
    {
        imageRotationList.at(1) = camer2->GetOneFrame();
    }
    else
    {
        ui->textBrowser->append("Camer2 is NULL");
    }
}

void MainWindow::on_pushButton_calibration_rotationCenter_offset_clicked()
{
    if(camer1 != NULL)
    {
        imageRotationList.at(2) = camer1->GetOneFrame();
    }
    else
    {
        ui->textBrowser->append("Camer1 is NULL");
    }

    if(camer2 != NULL)
    {
        imageRotationList.at(3) = camer2->GetOneFrame();
    }
    else
    {
        ui->textBrowser->append("Camer2 is NULL");
    }
}

void MainWindow::on_pushButton_calibration_translationX_curren_clicked()
{
    if(camer1 != NULL)
    {
        imageTranslationXList.at(0) = camer1->GetOneFrame();
    }
    else
    {
        ui->textBrowser->append("Camer1 is NULL");
    }

    if(camer2 != NULL)
    {
        imageTranslationXList.at(1) = camer2->GetOneFrame();
    }
    else
    {
        ui->textBrowser->append("Camer2 is NULL");
    }
}

void MainWindow::on_pushButton_calibration_translationX_offset_clicked()
{
    if(camer1 != NULL)
    {
        imageTranslationXList.at(2) = camer1->GetOneFrame();
    }
    else
    {
        ui->textBrowser->append("Camer1 is NULL");
    }

    if(camer2 != NULL)
    {
        imageTranslationXList.at(3) = camer2->GetOneFrame();
    }
    else
    {
        ui->textBrowser->append("Camer2 is NULL");
    }
}

void MainWindow::on_pushButton_calibration_translationY_curren_2_clicked()
{
    if(camer1 != NULL)
    {
        imageTranslationYList.at(0) = camer1->GetOneFrame();
    }
    else
    {
        ui->textBrowser->append("Camer1 is NULL");
    }

    if(camer2 != NULL)
    {
        imageTranslationYList.at(1) = camer2->GetOneFrame();
    }
    else
    {
        ui->textBrowser->append("Camer2 is NULL");
    }
}

void MainWindow::on_pushButton_calibration_translationY_offset_2_clicked()
{
    if(camer1 != NULL)
    {
        imageTranslationYList.at(2) = camer1->GetOneFrame();
    }
    else
    {
        ui->textBrowser->append("Camer1 is NULL");
    }

    if(camer2 != NULL)
    {
        imageTranslationYList.at(3) = camer2->GetOneFrame();
    }
    else
    {
        ui->textBrowser->append("Camer2 is NULL");
    }
}

void MainWindow::on_pushButton_run_clicked()
{
    if(camer1 == NULL)
    {
        ui->textBrowser->append("Camer1 is NULL");
        return ;
    }

    if(camer2 == NULL)
    {
        ui->textBrowser->append("Camer2 is NULL");
        return ;
    }

    imageCamer1 = camer1->GetOneFrame();
    if(!imageCamer1.data)
    {
        ui->textBrowser->append("Image1 is NULL");
        return ;
    }

    imageCamer2 = camer2->GetOneFrame();
    if(!imageCamer2.data)
    {
        ui->textBrowser->append("Image2 is NULL");
        return ;
    }
    std::map<Camera, Mat> product_images = { { Camera::LeftCamera, imageCamer1 } };
    product_images.insert({ Camera::RightCamera, imageCamer2 });

    Point2d translation;
    double rotation;

    bool operation_succeeded = visual_positioner.computeAlignParameters(product_images,rotation,translation);

    if(operation_succeeded)
    {
        ui->textBrowser->append(QString("Rotation:%1 \n Translation:(%2,%3)").arg(rotation).arg(QString::number(translation.x,'f',3)).arg(QString::number(translation.y,'f',3)));
        ui->scrollArea_camer1->setImage(product_images.at(Camera::LeftCamera));
        ui->scrollArea_camer2->setImage(product_images.at(Camera::RightCamera));
    }
    else
    {
        ui->textBrowser->append("ComputeAlignParameters False");
    }
}

void MainWindow::camer1ImageSlot(Mat img)
{
    ui->textBrowser->append("AAAAA");
    ui->scrollArea_camer1->setImage(img);
}

void MainWindow::camer2ImageSlot(Mat img)
{
    ui->textBrowser->append("BBBB");
    ui->scrollArea_camer2->setImage(img);
}

void MainWindow::on_pushButton_control_moveZero_clicked()
{
    for(int i=0; i<axisCount ; i++)
    {
        if(!dmc_check_done(0,i))
        {
           return ;
        }
    }

    controlMoveHome = std::thread(&MainWindow::controlMoveHomeSlot,this);
    controlMoveHome.detach();
}

void MainWindow::on_pushButton_moveP_negative_axisX_pressed()
{
    //0是负方向，1是正方向
    dmc_vmove(0,0,0);
}

void MainWindow::on_pushButton_moveP_negative_axisX_released()
{
    dmc_stop(0,0,0);
}

void MainWindow::on_pushButton_moveP_positive_axisX_pressed()
{
    dmc_vmove(0,0,1);
}

void MainWindow::on_pushButton_moveP_positive_axisX_released()
{
    dmc_stop(0,0,0);
}

void MainWindow::on_pushButton_moveP_negative_axisY_pressed()
{
    //0是负方向，1是正方向
    dmc_vmove(0,1,0);
}

void MainWindow::on_pushButton_moveP_negative_axisY_released()
{
    dmc_stop(0,1,0);
}

void MainWindow::on_pushButton_moveP_positive_axisY_pressed()
{
    dmc_vmove(0,1,1);
}

void MainWindow::on_pushButton_moveP_positive_axisY_released()
{
    dmc_stop(0,1,0);
}

void MainWindow::on_pushButton_moveP_negative_axisZ_pressed()
{
    //0是负方向，1是正方向
    dmc_vmove(0,2,0);
}

void MainWindow::on_pushButton_moveP_negative_axisZ_released()
{
    dmc_stop(0,2,0);
}

void MainWindow::on_pushButton_moveP_positive_axisZ_pressed()
{
    dmc_vmove(0,2,1);
}

void MainWindow::on_pushButton_moveP_positive_axisZ_released()
{
    dmc_stop(0,2,0);
}

void MainWindow::on_pushButton_moveP_negative_axisT_pressed()
{
    //0是负方向，1是正方向
    dmc_vmove(0,3,0);
}

void MainWindow::on_pushButton_moveP_negative_axisT_released()
{
    dmc_stop(0,3,0);
}

void MainWindow::on_pushButton_moveP_positive_axisT_pressed()
{
    dmc_vmove(0,3,1);
}

void MainWindow::on_pushButton_moveP_positive_axisT_released()
{
    dmc_stop(0,3,0);
}

void MainWindow::on_pushButton_moveP_negative_axisA_pressed()
{
    //0是负方向，1是正方向
    dmc_vmove(0,4,0);
}

void MainWindow::on_pushButton_moveP_negative_axisA_released()
{
    dmc_stop(0,4,0);
}

void MainWindow::on_pushButton_moveP_positive_axisA_pressed()
{
    dmc_vmove(0,4,1);
}

void MainWindow::on_pushButton_moveP_positive_axisA_released()
{
    dmc_stop(0,4,0);
}

void MainWindow::on_pushButton_control_record_load_clicked()
{
    loadProduct_Z = dmc_get_position(0,2);
    loadProduct_T = dmc_get_position(0,3);
}

void MainWindow::on_pushButton_control_move_load_clicked()
{
    if(!isAllAxisStop())
        return;
    moveLoadProductThread = std::thread(&MainWindow::moveLoadProductThreadSlot,this);
    moveLoadProductThread.detach();
}

void MainWindow::on_pushButton_control_record_platform_clicked()
{
    platform_Z = dmc_get_position(0,2);
    platform_T = dmc_get_position(0,3);
    platform_X = dmc_get_position(0,0);
    platform_Y = dmc_get_position(0,1);
    platform_A = dmc_get_position(0,4);
}

void MainWindow::on_pushButton_control_move_platform_clicked()
{
    if(!isAllAxisStop())
        return;
    movePlatformThread = std::thread(&MainWindow::movePlatformThreadSlot,this);
    movePlatformThread.detach();
}

void MainWindow::on_pushButton_control_record_detection_clicked()
{
    detection_X = dmc_get_position(0,0);
    detection_Y = dmc_get_position(0,1);
    detection_A = dmc_get_position(0,4);

}

void MainWindow::on_pushButton_control_move_detection_clicked()
{
    if(!isAllAxisStop())
        return;
    moveDetectionThread = std::thread(&MainWindow::moveDetectionThreadSlot,this);
    moveDetectionThread.detach();
}

void MainWindow::on_pushButton_control_record_blanking_clicked()
{
    blanking_Z = dmc_get_position(0,2);
    blanking_T = dmc_get_position(0,3);
}

void MainWindow::on_pushButton_control_move_blanking_clicked()
{
    if(!isAllAxisStop())
        return;
    moveBlankingThread = std::thread(&MainWindow::moveBlankingThreadSlot,this);
    moveBlankingThread.detach();
}


void MainWindow::on_pushButton_control_enable_toggled(bool checked)
{
    if(checked)
    {
        for(int i=0; i<axisCount ; i++)
            dmc_set_sevon_enable(0,i,1);

        ui->pushButton_control_enable->setText(QStringLiteral("使能关"));
    }
    else
    {
        for(int i=0; i<axisCount ; i++)
            dmc_set_sevon_enable(0,i,0);

        ui->pushButton_control_enable->setText(QStringLiteral("使能开"));
    }

}

void MainWindow::on_checkBox_clicked(bool checked)
{
    ui->pushButton_control_record_blanking->setEnabled(checked);
    ui->pushButton_control_record_load->setEnabled(checked);
    ui->pushButton_control_record_platform->setEnabled(checked);
    ui->pushButton_control_record_detection->setEnabled(checked);
}

void MainWindow::on_pushButton_move_axisX_clicked()
{
    double offset = ui->lineEdit_current_axisX->text().toDouble();
    if( offset == 0)
        return ;

    dmc_pmove(0,0,offset/factor,0);
}



void MainWindow::on_pushButton_control_proudctIO_toggled(bool checked)
{
    if(checked)
    {
        dmc_write_outbit(0,4,0);
        dmc_write_outbit(0,5,0);

        ui->pushButton_control_proudctIO->setText(QStringLiteral("产品吸气关"));
    }
    else
    {
        dmc_write_outbit(0,4,1);
        dmc_write_outbit(0,5,1);
        ui->pushButton_control_proudctIO->setText(QStringLiteral("产品吸气开"));
    }
}

void MainWindow::on_pushButton_control_platformIO_toggled(bool checked)
{
    if(checked)
    {
        dmc_write_outbit(0,6,0);


        ui->pushButton_control_platformIO->setText(QStringLiteral("平台吸气关"));
    }
    else
    {
        dmc_write_outbit(0,6,1);

        ui->pushButton_control_platformIO->setText(QStringLiteral("平台吸气开"));
    }
}

void MainWindow::on_pushButton_inProduct_clicked()
{
    if(!isAllAxisStop())
        return;
    inProductThread = std::thread(&MainWindow::inProductThreadSlot,this);
    inProductThread.detach();
}

void MainWindow::on_pushButton_outProduct_clicked()
{
    if(!isAllAxisStop())
        return;
    outProductThread = std::thread(&MainWindow::outProductThreadSlot,this);
    outProductThread.detach();
}


void MainWindow::on_pushButton_set_camer_ext_clicked()
{
    if(camer1 != NULL)
    {
        camer1->SetExpTime(ui->lineEdit_camer_ext->text().toInt());
        ui->textBrowser->append("Camer1 set ExpTime : True");
    }
}

void MainWindow::on_pushButton_set_camer_ext2_clicked()
{
    if(camer2 != NULL)
    {
        camer2->SetExpTime(ui->lineEdit_camer_ext2->text().toInt());
        ui->textBrowser->append("Camer1 set ExpTime : True");
    }
}

void MainWindow::on_pushButton_move_axisY_clicked()
{
    double offset = ui->lineEdit_current_axisY->text().toDouble();
    if( offset == 0)
        return ;

    dmc_pmove(0,1,offset/factor,0);
}



void MainWindow::on_pushButton_camerContinue_toggled(bool checked)
{
    if(checked)
    {
        isCamerContinue = true;
        camer1ContinueThread = std::thread(&MainWindow::camer1ContinueThreadSlot,this);
        camer1ContinueThread.detach();
        camer2ContinueThread = std::thread(&MainWindow::camer2ContinueThreadSlot,this);
        camer2ContinueThread.detach();

        ui->pushButton_camerContinue->setText(QStringLiteral("实时关"));
    }
    else
    {
        isCamerContinue = false;


        ui->pushButton_camerContinue->setText(QStringLiteral("实时开"));
    }
}



void MainWindow::on_pushButton_savePicture_clicked()
{
    cv::Mat camer1P = camer1->GetOneFrame();
    cv::Mat camer2P = camer2->GetOneFrame();

    cv::imwrite("./data/1.bmp",camer1P);
    cv::imwrite("./data/2.bmp",camer2P);
}
