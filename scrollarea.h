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

protected:
    bool eventFilter(QObject *obj, QEvent *evt);

 private:
    bool mMoveStart;
    bool mContinuousMove;
    QPoint mMousePoint;
    double radio = 1;
    int wheelIndex = 0;
    QSize imageSize;
    QList<QSize> sizeList;





public:
    Label_X *imageLabel;
    void setImage(cv::Mat img);
    void enlargedImage();
    void shrinkImage();

public:
    bool isDraw = false;
    QPoint pressPoint;
    QPoint releasePoint;



};

#endif // SCROLLAREA_H
