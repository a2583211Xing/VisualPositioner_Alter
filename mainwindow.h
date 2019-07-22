#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <VisualPositioner/VisualPositioner.h>
#include <opencv2/opencv.hpp>
#include <QHeaderView>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    VisualPositioner visual_positioner;
};

#endif // MAINWINDOW_H
