#include "scancellcornersdialog.h"
#include "ui_scancellcornersdialog.h"


ScanCellCornersDialog::ScanCellCornersDialog(const cv::Mat& boardImage, std::vector<cv::Point>& cellCorners, std::vector<cv::Point>& boardCorners, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScanCellCornersDialog)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for(int i=0; i<cellCorners.size();i++)
    {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row,0,new QTableWidgetItem(QString::number(cellCorners.at(i).x )));
        ui->tableWidget->setItem(row,1,new QTableWidgetItem(QString::number(cellCorners.at(i).y )));
        ui->tableWidget->setItem(row,2,new QTableWidgetItem(QString::number(boardCorners.at(i).x)));
        ui->tableWidget->setItem(row,3,new QTableWidgetItem(QString::number(boardCorners.at(i).y)));
    }



    image = boardImage;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(200);


}

ScanCellCornersDialog::~ScanCellCornersDialog()
{
    delete ui;
}

void ScanCellCornersDialog::update()
{
    timer->stop();
    ui->scrollArea->setImage(image);

}
