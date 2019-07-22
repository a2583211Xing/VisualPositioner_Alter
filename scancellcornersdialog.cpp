#include "scancellcornersdialog.h"
#include "ui_scancellcornersdialog.h"
#include <QDebug>


ScanCellCornersDialog::ScanCellCornersDialog(const cv::Mat& boardImage, std::vector<cv::Point>& cellCorners, std::vector<cv::Point>& boardCorners, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScanCellCornersDialog)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    cv::cvtColor(boardImage, image_show, CV_GRAY2BGR);

    cellCorners_temp = &cellCorners;
    boardCorners_temp = &boardCorners;



    for(int i=0; i<cellCorners.size();i++)
    {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row,0,new QTableWidgetItem(QString::number(cellCorners.at(i).x )));
        ui->tableWidget->setItem(row,1,new QTableWidgetItem(QString::number(cellCorners.at(i).y )));
        ui->tableWidget->setItem(row,2,new QTableWidgetItem(QString::number(boardCorners.at(i).x)));
        ui->tableWidget->setItem(row,3,new QTableWidgetItem(QString::number(boardCorners.at(i).y)));
        cv::circle(image_show,cellCorners.at(i),10,cv::Scalar(0,0,255),4);
    }







    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);


}

ScanCellCornersDialog::~ScanCellCornersDialog()
{
    delete ui;
}

void ScanCellCornersDialog::update()
{
    timer->stop();
    ui->scrollArea->setImage(image_show);
    isInitFinsh = true;

}

void ScanCellCornersDialog::on_tableWidget_cellChanged(int row, int column)
{
    if(isInitFinsh)
    {

        qDebug()<<"on_tableWidget_cellChanged";
        switch (column) {
        case 0:
            cellCorners_temp->at(row).x = ui->tableWidget->item(row,column)->text().toInt();
            break;
        case 1:
            cellCorners_temp->at(row).y = ui->tableWidget->item(row,column)->text().toInt();
            break;
        case 2:
            boardCorners_temp->at(row).x = ui->tableWidget->item(row,column)->text().toInt();
            break;
        case 3:
            boardCorners_temp->at(row).y = ui->tableWidget->item(row,column)->text().toInt();
            break;
        default:
            break;
        }


    }
}
