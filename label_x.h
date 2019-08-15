#ifndef LABEL_X_H
#define LABEL_X_H


#include <QLabel>
#include <QMouseEvent>
#include <QDebug>
#include <opencv2/opencv.hpp>

class Label_X : public QLabel
{
    Q_OBJECT
public:
    Label_X(QWidget *parent = 0);

protected:
    bool eventFilter(QObject *obj, QEvent *evt);

public:
    cv::Mat img;
    cv::Mat imageShow;
    bool isDraw = false;
    QPoint pressPoint;
    QPoint releasePoint;
    QPoint leftUP;
    QPoint rightDown;

public:
    void setImage(cv::Mat img);

private:
    QImage MatToQImage(const cv::Mat &mat);

};

#endif // LABEL_X_H
