#include "label_x.h"

Label_X::Label_X(QWidget *parent): QLabel(parent)
{
    installEventFilter(this);
}

bool Label_X::eventFilter(QObject *obj, QEvent *evt)
{
    if(isDraw)
    {

        QMouseEvent* mouseEvent = (QMouseEvent*) evt;
        if(evt->type() == QEvent::MouseButtonPress)
        {

            if ((mouseEvent->buttons() & Qt::LeftButton))
            {
                pressPoint = mouseEvent->pos();

            }
        }
        if (evt->type() == QEvent::MouseMove)
        {
            if ((mouseEvent->buttons() & Qt::LeftButton))
            {
                releasePoint = mouseEvent->pos();

                double radio = (double)this->size().width() / (double)img.cols;

                leftUP       = pressPoint/radio;
                rightDown = releasePoint/radio;

                cv::Mat temp = imageShow.clone();

                cv::rectangle(temp,cv::Point(leftUP.x(),leftUP.y()),cv::Point(rightDown.x(),rightDown.y()),cv::Scalar(0,255,0),1,cv::LINE_8);

                this->setPixmap(QPixmap::fromImage(MatToQImage(temp)));

            }
        }

    }

    return QLabel::eventFilter(obj,evt);
}

void Label_X::setImage(cv::Mat img)
{
    this->img = img;
    if(this->img.channels() == 1){
        cv::cvtColor(img, imageShow, CV_GRAY2BGR);
    }
    else{
        imageShow = img.clone();
    }

    this->setPixmap(QPixmap::fromImage(MatToQImage(imageShow)));
}

QImage Label_X::MatToQImage(const cv::Mat &mat)
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
