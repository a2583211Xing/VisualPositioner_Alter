#ifndef SCROLLAREA_H
#define SCROLLAREA_H


#include <QScrollArea>
#include <QPoint>
#include "label_x.h"
#include <QGridLayout>
#include <opencv2/opencv.hpp>

class ScrollArea : public QScrollArea
{
    Q_OBJECT

public:
    ScrollArea(QWidget* parent =NULL);
    ~ScrollArea();


public:
    Label_X *imageLabel;
    void setImage(cv::Mat img);
    void enlargedImage();
    void shrinkImage();

public:
    void setDraw(bool temp);


protected:
    bool eventFilter(QObject *obj, QEvent *evt);

private:
    bool isDraw = false;
    bool mMoveStart;
    bool mContinuousMove;
    QPoint mMousePoint;
    double radio = 1;
    int wheelIndex = 0;
    QSize imageSize;
    QList<QSize> sizeList;

signals:
    void drawRectSignal(cv::Rect);



};

#endif // SCROLLAREA_H
