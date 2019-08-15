#include "camer_hk.h"



Camer_HK::Camer_HK()
{
    handle = NULL;
    InitLog("Camera","1.0.0");

}

bool Camer_HK::OpenCamera(QString ips)
{
    unsigned int ipt = 0;
    QStringList adressList = ips.split(".");
    for(int i = 0; i<adressList.count();i++)
    {
        QString address = adressList.at(i);
        ipt += address.toInt()<<((3-i)*8);
    }

    return OpenCamera(ipt);
}

bool Camer_HK::OpenCamera(unsigned int cameraIntIP)
{
    int nRet = 0;
    int nDeviceNum = -1;
    MV_CC_DEVICE_INFO_LIST stDeviceList;

    memset( &stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST) );



    // 1. 枚举设备
    nRet = MV_CC_EnumDevices( MV_GIGE_DEVICE | MV_USB_DEVICE, &stDeviceList );
    if ( MV_OK != nRet )
    {
        Log_Error( logger, "MV_CC_EnumDevices fail! nRet %x", nRet );

        return false;
    }
    if (stDeviceList.nDeviceNum < 0)
    {
        Log_Error( logger, "Device Num less than 0");

        return false;
    }

    for(int i =0; i < (int)stDeviceList.nDeviceNum; i++ )
    {
        if(cameraIntIP == stDeviceList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp)
        {
            nDeviceNum = i;
            break;
        }
        continue;
    }

    if(nDeviceNum < 0)
    {
        Log_Error( logger, "Not Find Camera Device....");
        return false;
    }

    // 2. 选择设备并创建句柄
    nRet = MV_CC_CreateHandle( &handle, stDeviceList.pDeviceInfo[nDeviceNum] );
    if (MV_OK != nRet)
    {
        Log_Error( logger, "MV_CC_CreateHandle fail! nRet %x", nRet );

        return false;
    }

    // 3.打开设备
    nRet = MV_CC_OpenDevice( handle );
    if (MV_OK != nRet)
    {
        Log_Error( logger, "MV_CC_OpenDevice fail! nRet %x", nRet );

        return false;
    }
    //获取相机图像基本信息

    MV_IMAGE_BASIC_INFO mstruBasicInfo = {0};
    nRet = MV_CC_GetImageInfo(handle, &mstruBasicInfo);
    img_width = mstruBasicInfo.nWidthValue;
    img_height = mstruBasicInfo.nHeightValue;
    imageData = (uchar * )malloc(sizeof(uchar) * img_width * img_height  );


    //设置图片格式
    nRet = MV_CC_SetPixelFormat( handle, PixelType_Gvsp_Mono8 );
    if (MV_OK != nRet)
    {
        Log_Error( logger, "MV_CC_SetPixelFormat fail! nRet %x", nRet );

        return false;
    }
    //打开触发模式
    nRet = MV_CC_SetTriggerMode( handle, MV_TRIGGER_MODE_ON ) ;
    if (MV_OK != nRet)
    {
        Log_Error( logger, "MV_CC_SetTriggerMode fail! nRet %x", nRet );

        return false;
    }
    //设置软触发
    nRet = MV_CC_SetTriggerSource( handle, MV_TRIGGER_SOURCE_SOFTWARE );
    if (MV_OK != nRet)
    {
        Log_Error( logger, "MV_CC_SetTriggerSource fail! nRet %x", nRet );

        return false;
    }
    //注册回调函数
    nRet = MV_CC_RegisterImageCallBack( handle, ImageCallBack, this );
    if (MV_OK != nRet)
    {
        Log_Error( logger, "MV_CC_RegisterImageCallBack fail! nRet %x", nRet );

        return false;
    }
    //开始抓图
    nRet = MV_CC_StartGrabbing( handle );
    if( MV_OK != nRet )
    {
        Log_Error( logger, "MV_CC_StartGrabbing fail! nRet %x", nRet );

        return false;
    }




    Log_Debug( logger, "open camera success");
    return true;

}

bool Camer_HK::SetExpTime(int ExpTime)
{
    int nRet = 0;

    nRet = MV_CC_SetExposureTime( handle, 1.0f*ExpTime );
    if ( MV_OK != nRet )
    {
        Log_Error( logger, "MV_CC_SetExposureTime fail! nRet %x", nRet );

        return false;
    }

    return true;

    Log_Debug( logger, "Set Exposure Time end....");
}

Mat Camer_HK::GetOneFrame()
{
    Log_Debug(logger,"GetOneFrame Start");
    Mat img;
    long tm_start,tm_end;
    int nRet = 0;


    nRet = MV_CC_TriggerSoftwareExecute( handle );
    if ( MV_OK != nRet )
    {
        Log_Error( logger, "MV_CC_TriggerSoftwareExecute fail! nRet %x", nRet );

        return img;
    }


    tm_start = clock();
    while ( true)
    {
        tm_end = clock();
        if( tm_end - tm_start > 3000)
        {
            Log_Error( logger, " Get Camera Photo out of time" );
            return img;
        }

        if(takePhotoFinish == 1)
        {

            Log_Debug( logger, "takePhotoFinish");
            img = Mat(img_height, img_width, CV_8UC1,imageData );

            break;
        }
    }




    takePhotoFinish = 0;
    Log_Debug( logger, "Get One Frame End");
    return img.clone();
}

uchar *Camer_HK::getImageData()
{
    if( nullptr != imageData )
    {
        return imageData;
    }
    return nullptr;
}

QImage Camer_HK::MatToQImage(const Mat &mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, (int)mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {

        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, (int)mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {

        return QImage();
    }
}

void Camer_HK::ImageCallBack(unsigned char *pData, MV_FRAME_OUT_INFO *pFrameInfo, void *pUser)
{

    Camer_HK  *cameraData = (Camer_HK * )pUser;
    Log_Debug(cameraData->logger,"ImageCallBack Start");
    int imageSize = pFrameInfo->nFrameLen;

    memcpy( cameraData->imageData, pData, imageSize );


    cameraData->takePhotoFinish = 1;
    Log_Debug(cameraData->logger,"ImageCallBack End")

}

void Camer_HK::InitLog(QString logname, QString sys_version)
{
    QDir dir;
    bool exist = dir.exists( "./debug_message" );
    if(!exist)
    {
        dir.mkpath( "./debug_message" );
    }

    logger = Logger::getLogger( "Camera" );
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
