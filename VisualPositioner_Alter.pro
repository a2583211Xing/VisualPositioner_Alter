#-------------------------------------------------
#
# Project created by QtCreator 2019-07-22T08:34:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VisualPositioner_Alter
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    VisualPositioner/CornerDetection.cpp \
    VisualPositioner/CrossMarkDetection.cpp \
    VisualPositioner/Utils.cpp \
    VisualPositioner/VisualPositioner.cpp \
    label_x.cpp \
    scrollarea.cpp \
    scancellcornersdialog.cpp

HEADERS += \
        mainwindow.h \
    VisualPositioner/CornerDetection.h \
    VisualPositioner/CrossMarkDetection.h \
    VisualPositioner/Enums.h \
    VisualPositioner/Utils.h \
    VisualPositioner/VisualPositioner.h \
    label_x.h \
    scrollarea.h \
    scancellcornersdialog.h

FORMS += \
        mainwindow.ui \
    scancellcornersdialog.ui






#需修改此路径
ProPATH = F:/code/3rdparty


#opencv344
INCLUDEPATH += $${ProPATH}/opencv344/include
LIBS += -L$${ProPATH}/opencv344/lib/ \
        -lopencv_world344 \
        -lopencv_world344d\


#json
INCLUDEPATH += $${ProPATH}/cpp-json
