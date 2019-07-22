#ifndef SCANCELLCORNERSDIALOG_H
#define SCANCELLCORNERSDIALOG_H

#include <QDialog>
#include <QTableWidgetItem>
#include <QTimer>
#include <opencv2/opencv.hpp>

namespace Ui {
class ScanCellCornersDialog;
}

class ScanCellCornersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScanCellCornersDialog(const cv::Mat& boardImage,std::vector<cv::Point>& cellCorners, std::vector<cv::Point>& boardCorners,QWidget *parent = 0);

    ~ScanCellCornersDialog();

private:
    Ui::ScanCellCornersDialog *ui;

    cv::Mat image_show;
    QTimer *timer;
    bool isInitFinsh = false;

    std::vector<cv::Point> *cellCorners_temp;
    std::vector<cv::Point>  *boardCorners_temp;

private slots:
    void update();
    void on_tableWidget_cellChanged(int row, int column);
};

#endif // SCANCELLCORNERSDIALOG_H
