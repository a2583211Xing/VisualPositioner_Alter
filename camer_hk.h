#ifndef CAMER_HK_H
#define CAMER_HK_H

#include <windows.h>
#include <time.h>
#include <process.h>
#include <QDateTime>
#include <QDir>
#include <QList>
#include <QString>
#include <QDebug>
#include <string.h>
#include <QDialog>
#include <QtNetwork/QTcpServer>
#include <opencv2/opencv.hpp>
#include <MvCameraControl.h>
#include <log4cxx/xLog.h>

using namespace cv;


class Camer_HK
{
public:
    Camer_HK();

    bool    OpenCamera(QString);

    bool    OpenCamera(unsigned int cameraIntIP);

    bool    SetExpTime(int ExpTime);

    Mat     GetOneFrame();

    uchar *getImageData();

    QImage MatToQImage(const Mat &mat);


private:

    static void __stdcall ImageCallBack(unsigned char * pData, MV_FRAME_OUT_INFO* pFrameInfo, void* pUser);

    void    InitLog(QString logname, QString sys_version);
private:
    log4cxx::LoggerPtr  logger;

public:
    uchar * imageData ;
    void * handle;
    int     takePhotoFinish;

    int img_width  = 3072;
    int img_height = 2048;



};

#endif // CAMER_HK_H
