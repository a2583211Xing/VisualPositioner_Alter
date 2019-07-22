#ifndef SCANCELLCORNERSDIALOG_H
#define SCANCELLCORNERSDIALOG_H

#include <QDialog>
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
};

#endif // SCANCELLCORNERSDIALOG_H
