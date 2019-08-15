/********************************************************************************
** Form generated from reading UI file 'scancellcornersdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCANCELLCORNERSDIALOG_H
#define UI_SCANCELLCORNERSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>
#include <scrollarea.h>

QT_BEGIN_NAMESPACE

class Ui_ScanCellCornersDialog
{
public:
    QGridLayout *gridLayout;
    ScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QTableWidget *tableWidget;
    QPushButton *pushButton;

    void setupUi(QDialog *ScanCellCornersDialog)
    {
        if (ScanCellCornersDialog->objectName().isEmpty())
            ScanCellCornersDialog->setObjectName(QStringLiteral("ScanCellCornersDialog"));
        ScanCellCornersDialog->resize(945, 569);
        gridLayout = new QGridLayout(ScanCellCornersDialog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        scrollArea = new ScrollArea(ScanCellCornersDialog);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 598, 549));
        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout->addWidget(scrollArea, 0, 0, 2, 1);

        tableWidget = new QTableWidget(ScanCellCornersDialog);
        if (tableWidget->columnCount() < 4)
            tableWidget->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setMinimumSize(QSize(321, 0));
        tableWidget->setMaximumSize(QSize(321, 16777215));

        gridLayout->addWidget(tableWidget, 0, 1, 1, 1);

        pushButton = new QPushButton(ScanCellCornersDialog);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setMinimumSize(QSize(0, 35));

        gridLayout->addWidget(pushButton, 1, 1, 1, 1);


        retranslateUi(ScanCellCornersDialog);
        QObject::connect(pushButton, SIGNAL(clicked()), ScanCellCornersDialog, SLOT(close()));

        QMetaObject::connectSlotsByName(ScanCellCornersDialog);
    } // setupUi

    void retranslateUi(QDialog *ScanCellCornersDialog)
    {
        ScanCellCornersDialog->setWindowTitle(QApplication::translate("ScanCellCornersDialog", "Dialog", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("ScanCellCornersDialog", "\345\203\217\347\264\240\345\235\220\346\240\207 X", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("ScanCellCornersDialog", "\345\203\217\347\264\240\345\235\220\346\240\207 Y", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("ScanCellCornersDialog", "\347\211\251\347\220\206\345\235\220\346\240\207 X", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("ScanCellCornersDialog", "\347\211\251\347\220\206\345\235\220\346\240\207 Y", nullptr));
        pushButton->setText(QApplication::translate("ScanCellCornersDialog", "\347\241\256\350\256\244", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ScanCellCornersDialog: public Ui_ScanCellCornersDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCANCELLCORNERSDIALOG_H
