#include "scancellcornersdialog.h"
#include "ui_scancellcornersdialog.h"

ScanCellCornersDialog::ScanCellCornersDialog(const cv::Mat& boardImage, std::vector<cv::Point>& cellCorners, std::vector<cv::Point>& boardCorners, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScanCellCornersDialog)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    qDebug()<<"cellCorners:"<<cellCorners.size();



}

ScanCellCornersDialog::~ScanCellCornersDialog()
{
    delete ui;
}
