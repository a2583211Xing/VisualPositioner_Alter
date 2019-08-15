/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[87];
    char stringdata0[3185];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 17), // "camer1ImageSignal"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 7), // "cv::Mat"
QT_MOC_LITERAL(4, 38, 17), // "camer2ImageSignal"
QT_MOC_LITERAL(5, 56, 15), // "currenAxisPoint"
QT_MOC_LITERAL(6, 72, 13), // "drawRectSlot1"
QT_MOC_LITERAL(7, 86, 8), // "cv::Rect"
QT_MOC_LITERAL(8, 95, 48), // "on_pushButton_calibration_reg..."
QT_MOC_LITERAL(9, 144, 48), // "on_pushButton_calibration_rot..."
QT_MOC_LITERAL(10, 193, 54), // "on_pushButton_calibration_tra..."
QT_MOC_LITERAL(11, 248, 47), // "on_comboBox_line1_direction_c..."
QT_MOC_LITERAL(12, 296, 5), // "index"
QT_MOC_LITERAL(13, 302, 47), // "on_comboBox_line2_direction_c..."
QT_MOC_LITERAL(14, 350, 49), // "on_comboBox_line1_direction_2..."
QT_MOC_LITERAL(15, 400, 49), // "on_comboBox_line2_direction_2..."
QT_MOC_LITERAL(16, 450, 41), // "on_spinBox_line1_threshold_2_..."
QT_MOC_LITERAL(17, 492, 4), // "arg1"
QT_MOC_LITERAL(18, 497, 41), // "on_spinBox_line2_threshold_2_..."
QT_MOC_LITERAL(19, 539, 39), // "on_spinBox_line1_threshold_va..."
QT_MOC_LITERAL(20, 579, 39), // "on_spinBox_line2_threshold_va..."
QT_MOC_LITERAL(21, 619, 26), // "on_pushButton_test_clicked"
QT_MOC_LITERAL(22, 646, 32), // "on_pushButton_line1_roi1_clicked"
QT_MOC_LITERAL(23, 679, 32), // "on_pushButton_line1_roi2_clicked"
QT_MOC_LITERAL(24, 712, 32), // "on_pushButton_line2_roi1_clicked"
QT_MOC_LITERAL(25, 745, 32), // "on_pushButton_line2_roi2_clicked"
QT_MOC_LITERAL(26, 778, 34), // "on_pushButton_line1_roi1_2_cl..."
QT_MOC_LITERAL(27, 813, 34), // "on_pushButton_line1_roi2_2_cl..."
QT_MOC_LITERAL(28, 848, 34), // "on_pushButton_line2_roi1_2_cl..."
QT_MOC_LITERAL(29, 883, 34), // "on_pushButton_line2_roi2_2_cl..."
QT_MOC_LITERAL(30, 918, 38), // "on_pushButton_StandardPositio..."
QT_MOC_LITERAL(31, 957, 33), // "on_pushButton_camer1_Test_cli..."
QT_MOC_LITERAL(32, 991, 33), // "on_pushButton_camer2_Test_cli..."
QT_MOC_LITERAL(33, 1025, 55), // "on_pushButton_calibration_rot..."
QT_MOC_LITERAL(34, 1081, 31), // "on_pushButton_save_Json_clicked"
QT_MOC_LITERAL(35, 1113, 34), // "on_pushButton_save_setting_cl..."
QT_MOC_LITERAL(36, 1148, 55), // "on_pushButton_calibration_rot..."
QT_MOC_LITERAL(37, 1204, 53), // "on_pushButton_calibration_tra..."
QT_MOC_LITERAL(38, 1258, 53), // "on_pushButton_calibration_tra..."
QT_MOC_LITERAL(39, 1312, 55), // "on_pushButton_calibration_tra..."
QT_MOC_LITERAL(40, 1368, 55), // "on_pushButton_calibration_tra..."
QT_MOC_LITERAL(41, 1424, 25), // "on_pushButton_run_clicked"
QT_MOC_LITERAL(42, 1450, 15), // "camer1ImageSlot"
QT_MOC_LITERAL(43, 1466, 15), // "camer2ImageSlot"
QT_MOC_LITERAL(44, 1482, 19), // "currenAxisPointSlot"
QT_MOC_LITERAL(45, 1502, 38), // "on_pushButton_control_moveZer..."
QT_MOC_LITERAL(46, 1541, 42), // "on_pushButton_moveP_negative_..."
QT_MOC_LITERAL(47, 1584, 43), // "on_pushButton_moveP_negative_..."
QT_MOC_LITERAL(48, 1628, 42), // "on_pushButton_moveP_positive_..."
QT_MOC_LITERAL(49, 1671, 43), // "on_pushButton_moveP_positive_..."
QT_MOC_LITERAL(50, 1715, 42), // "on_pushButton_moveP_negative_..."
QT_MOC_LITERAL(51, 1758, 43), // "on_pushButton_moveP_negative_..."
QT_MOC_LITERAL(52, 1802, 42), // "on_pushButton_moveP_positive_..."
QT_MOC_LITERAL(53, 1845, 43), // "on_pushButton_moveP_positive_..."
QT_MOC_LITERAL(54, 1889, 42), // "on_pushButton_moveP_negative_..."
QT_MOC_LITERAL(55, 1932, 43), // "on_pushButton_moveP_negative_..."
QT_MOC_LITERAL(56, 1976, 42), // "on_pushButton_moveP_positive_..."
QT_MOC_LITERAL(57, 2019, 43), // "on_pushButton_moveP_positive_..."
QT_MOC_LITERAL(58, 2063, 42), // "on_pushButton_moveP_negative_..."
QT_MOC_LITERAL(59, 2106, 43), // "on_pushButton_moveP_negative_..."
QT_MOC_LITERAL(60, 2150, 42), // "on_pushButton_moveP_positive_..."
QT_MOC_LITERAL(61, 2193, 43), // "on_pushButton_moveP_positive_..."
QT_MOC_LITERAL(62, 2237, 42), // "on_pushButton_moveP_negative_..."
QT_MOC_LITERAL(63, 2280, 43), // "on_pushButton_moveP_negative_..."
QT_MOC_LITERAL(64, 2324, 42), // "on_pushButton_moveP_positive_..."
QT_MOC_LITERAL(65, 2367, 43), // "on_pushButton_moveP_positive_..."
QT_MOC_LITERAL(66, 2411, 41), // "on_pushButton_control_record_..."
QT_MOC_LITERAL(67, 2453, 39), // "on_pushButton_control_move_lo..."
QT_MOC_LITERAL(68, 2493, 45), // "on_pushButton_control_record_..."
QT_MOC_LITERAL(69, 2539, 43), // "on_pushButton_control_move_pl..."
QT_MOC_LITERAL(70, 2583, 46), // "on_pushButton_control_record_..."
QT_MOC_LITERAL(71, 2630, 44), // "on_pushButton_control_move_de..."
QT_MOC_LITERAL(72, 2675, 45), // "on_pushButton_control_record_..."
QT_MOC_LITERAL(73, 2721, 43), // "on_pushButton_control_move_bl..."
QT_MOC_LITERAL(74, 2765, 36), // "on_pushButton_control_enable_..."
QT_MOC_LITERAL(75, 2802, 7), // "checked"
QT_MOC_LITERAL(76, 2810, 19), // "on_checkBox_clicked"
QT_MOC_LITERAL(77, 2830, 32), // "on_pushButton_move_axisX_clicked"
QT_MOC_LITERAL(78, 2863, 39), // "on_pushButton_control_proudct..."
QT_MOC_LITERAL(79, 2903, 40), // "on_pushButton_control_platfor..."
QT_MOC_LITERAL(80, 2944, 31), // "on_pushButton_inProduct_clicked"
QT_MOC_LITERAL(81, 2976, 32), // "on_pushButton_outProduct_clicked"
QT_MOC_LITERAL(82, 3009, 35), // "on_pushButton_set_camer_ext_c..."
QT_MOC_LITERAL(83, 3045, 36), // "on_pushButton_set_camer_ext2_..."
QT_MOC_LITERAL(84, 3082, 32), // "on_pushButton_move_axisY_clicked"
QT_MOC_LITERAL(85, 3115, 35), // "on_pushButton_camerContinue_t..."
QT_MOC_LITERAL(86, 3151, 33) // "on_pushButton_savePicture_cli..."

    },
    "MainWindow\0camer1ImageSignal\0\0cv::Mat\0"
    "camer2ImageSignal\0currenAxisPoint\0"
    "drawRectSlot1\0cv::Rect\0"
    "on_pushButton_calibration_registerCamera_clicked\0"
    "on_pushButton_calibration_rotationCenter_clicked\0"
    "on_pushButton_calibration_translationDirection_clicked\0"
    "on_comboBox_line1_direction_currentIndexChanged\0"
    "index\0on_comboBox_line2_direction_currentIndexChanged\0"
    "on_comboBox_line1_direction_2_currentIndexChanged\0"
    "on_comboBox_line2_direction_2_currentIndexChanged\0"
    "on_spinBox_line1_threshold_2_valueChanged\0"
    "arg1\0on_spinBox_line2_threshold_2_valueChanged\0"
    "on_spinBox_line1_threshold_valueChanged\0"
    "on_spinBox_line2_threshold_valueChanged\0"
    "on_pushButton_test_clicked\0"
    "on_pushButton_line1_roi1_clicked\0"
    "on_pushButton_line1_roi2_clicked\0"
    "on_pushButton_line2_roi1_clicked\0"
    "on_pushButton_line2_roi2_clicked\0"
    "on_pushButton_line1_roi1_2_clicked\0"
    "on_pushButton_line1_roi2_2_clicked\0"
    "on_pushButton_line2_roi1_2_clicked\0"
    "on_pushButton_line2_roi2_2_clicked\0"
    "on_pushButton_StandardPosition_clicked\0"
    "on_pushButton_camer1_Test_clicked\0"
    "on_pushButton_camer2_Test_clicked\0"
    "on_pushButton_calibration_rotationCenter_curren_clicked\0"
    "on_pushButton_save_Json_clicked\0"
    "on_pushButton_save_setting_clicked\0"
    "on_pushButton_calibration_rotationCenter_offset_clicked\0"
    "on_pushButton_calibration_translationX_curren_clicked\0"
    "on_pushButton_calibration_translationX_offset_clicked\0"
    "on_pushButton_calibration_translationY_curren_2_clicked\0"
    "on_pushButton_calibration_translationY_offset_2_clicked\0"
    "on_pushButton_run_clicked\0camer1ImageSlot\0"
    "camer2ImageSlot\0currenAxisPointSlot\0"
    "on_pushButton_control_moveZero_clicked\0"
    "on_pushButton_moveP_negative_axisX_pressed\0"
    "on_pushButton_moveP_negative_axisX_released\0"
    "on_pushButton_moveP_positive_axisX_pressed\0"
    "on_pushButton_moveP_positive_axisX_released\0"
    "on_pushButton_moveP_negative_axisY_pressed\0"
    "on_pushButton_moveP_negative_axisY_released\0"
    "on_pushButton_moveP_positive_axisY_pressed\0"
    "on_pushButton_moveP_positive_axisY_released\0"
    "on_pushButton_moveP_negative_axisZ_pressed\0"
    "on_pushButton_moveP_negative_axisZ_released\0"
    "on_pushButton_moveP_positive_axisZ_pressed\0"
    "on_pushButton_moveP_positive_axisZ_released\0"
    "on_pushButton_moveP_negative_axisT_pressed\0"
    "on_pushButton_moveP_negative_axisT_released\0"
    "on_pushButton_moveP_positive_axisT_pressed\0"
    "on_pushButton_moveP_positive_axisT_released\0"
    "on_pushButton_moveP_negative_axisA_pressed\0"
    "on_pushButton_moveP_negative_axisA_released\0"
    "on_pushButton_moveP_positive_axisA_pressed\0"
    "on_pushButton_moveP_positive_axisA_released\0"
    "on_pushButton_control_record_load_clicked\0"
    "on_pushButton_control_move_load_clicked\0"
    "on_pushButton_control_record_platform_clicked\0"
    "on_pushButton_control_move_platform_clicked\0"
    "on_pushButton_control_record_detection_clicked\0"
    "on_pushButton_control_move_detection_clicked\0"
    "on_pushButton_control_record_blanking_clicked\0"
    "on_pushButton_control_move_blanking_clicked\0"
    "on_pushButton_control_enable_toggled\0"
    "checked\0on_checkBox_clicked\0"
    "on_pushButton_move_axisX_clicked\0"
    "on_pushButton_control_proudctIO_toggled\0"
    "on_pushButton_control_platformIO_toggled\0"
    "on_pushButton_inProduct_clicked\0"
    "on_pushButton_outProduct_clicked\0"
    "on_pushButton_set_camer_ext_clicked\0"
    "on_pushButton_set_camer_ext2_clicked\0"
    "on_pushButton_move_axisY_clicked\0"
    "on_pushButton_camerContinue_toggled\0"
    "on_pushButton_savePicture_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      80,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  414,    2, 0x06 /* Public */,
       4,    1,  417,    2, 0x06 /* Public */,
       5,    5,  420,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,  431,    2, 0x08 /* Private */,
       8,    0,  434,    2, 0x08 /* Private */,
       9,    0,  435,    2, 0x08 /* Private */,
      10,    0,  436,    2, 0x08 /* Private */,
      11,    1,  437,    2, 0x08 /* Private */,
      13,    1,  440,    2, 0x08 /* Private */,
      14,    1,  443,    2, 0x08 /* Private */,
      15,    1,  446,    2, 0x08 /* Private */,
      16,    1,  449,    2, 0x08 /* Private */,
      18,    1,  452,    2, 0x08 /* Private */,
      19,    1,  455,    2, 0x08 /* Private */,
      20,    1,  458,    2, 0x08 /* Private */,
      21,    0,  461,    2, 0x08 /* Private */,
      22,    0,  462,    2, 0x08 /* Private */,
      23,    0,  463,    2, 0x08 /* Private */,
      24,    0,  464,    2, 0x08 /* Private */,
      25,    0,  465,    2, 0x08 /* Private */,
      26,    0,  466,    2, 0x08 /* Private */,
      27,    0,  467,    2, 0x08 /* Private */,
      28,    0,  468,    2, 0x08 /* Private */,
      29,    0,  469,    2, 0x08 /* Private */,
      30,    0,  470,    2, 0x08 /* Private */,
      31,    0,  471,    2, 0x08 /* Private */,
      32,    0,  472,    2, 0x08 /* Private */,
      33,    0,  473,    2, 0x08 /* Private */,
      34,    0,  474,    2, 0x08 /* Private */,
      35,    0,  475,    2, 0x08 /* Private */,
      36,    0,  476,    2, 0x08 /* Private */,
      37,    0,  477,    2, 0x08 /* Private */,
      38,    0,  478,    2, 0x08 /* Private */,
      39,    0,  479,    2, 0x08 /* Private */,
      40,    0,  480,    2, 0x08 /* Private */,
      41,    0,  481,    2, 0x08 /* Private */,
      42,    1,  482,    2, 0x08 /* Private */,
      43,    1,  485,    2, 0x08 /* Private */,
      44,    5,  488,    2, 0x08 /* Private */,
      45,    0,  499,    2, 0x08 /* Private */,
      46,    0,  500,    2, 0x08 /* Private */,
      47,    0,  501,    2, 0x08 /* Private */,
      48,    0,  502,    2, 0x08 /* Private */,
      49,    0,  503,    2, 0x08 /* Private */,
      50,    0,  504,    2, 0x08 /* Private */,
      51,    0,  505,    2, 0x08 /* Private */,
      52,    0,  506,    2, 0x08 /* Private */,
      53,    0,  507,    2, 0x08 /* Private */,
      54,    0,  508,    2, 0x08 /* Private */,
      55,    0,  509,    2, 0x08 /* Private */,
      56,    0,  510,    2, 0x08 /* Private */,
      57,    0,  511,    2, 0x08 /* Private */,
      58,    0,  512,    2, 0x08 /* Private */,
      59,    0,  513,    2, 0x08 /* Private */,
      60,    0,  514,    2, 0x08 /* Private */,
      61,    0,  515,    2, 0x08 /* Private */,
      62,    0,  516,    2, 0x08 /* Private */,
      63,    0,  517,    2, 0x08 /* Private */,
      64,    0,  518,    2, 0x08 /* Private */,
      65,    0,  519,    2, 0x08 /* Private */,
      66,    0,  520,    2, 0x08 /* Private */,
      67,    0,  521,    2, 0x08 /* Private */,
      68,    0,  522,    2, 0x08 /* Private */,
      69,    0,  523,    2, 0x08 /* Private */,
      70,    0,  524,    2, 0x08 /* Private */,
      71,    0,  525,    2, 0x08 /* Private */,
      72,    0,  526,    2, 0x08 /* Private */,
      73,    0,  527,    2, 0x08 /* Private */,
      74,    1,  528,    2, 0x08 /* Private */,
      76,    1,  531,    2, 0x08 /* Private */,
      77,    0,  534,    2, 0x08 /* Private */,
      78,    1,  535,    2, 0x08 /* Private */,
      79,    1,  538,    2, 0x08 /* Private */,
      80,    0,  541,    2, 0x08 /* Private */,
      81,    0,  542,    2, 0x08 /* Private */,
      82,    0,  543,    2, 0x08 /* Private */,
      83,    0,  544,    2, 0x08 /* Private */,
      84,    0,  545,    2, 0x08 /* Private */,
      85,    1,  546,    2, 0x08 /* Private */,
      86,    0,  549,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,    2,    2,    2,    2,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 7,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,    2,    2,    2,    2,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   75,
    QMetaType::Void, QMetaType::Bool,   75,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   75,
    QMetaType::Void, QMetaType::Bool,   75,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   75,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->camer1ImageSignal((*reinterpret_cast< cv::Mat(*)>(_a[1]))); break;
        case 1: _t->camer2ImageSignal((*reinterpret_cast< cv::Mat(*)>(_a[1]))); break;
        case 2: _t->currenAxisPoint((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5]))); break;
        case 3: _t->drawRectSlot1((*reinterpret_cast< cv::Rect(*)>(_a[1]))); break;
        case 4: _t->on_pushButton_calibration_registerCamera_clicked(); break;
        case 5: _t->on_pushButton_calibration_rotationCenter_clicked(); break;
        case 6: _t->on_pushButton_calibration_translationDirection_clicked(); break;
        case 7: _t->on_comboBox_line1_direction_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->on_comboBox_line2_direction_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->on_comboBox_line1_direction_2_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->on_comboBox_line2_direction_2_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->on_spinBox_line1_threshold_2_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->on_spinBox_line2_threshold_2_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->on_spinBox_line1_threshold_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->on_spinBox_line2_threshold_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->on_pushButton_test_clicked(); break;
        case 16: _t->on_pushButton_line1_roi1_clicked(); break;
        case 17: _t->on_pushButton_line1_roi2_clicked(); break;
        case 18: _t->on_pushButton_line2_roi1_clicked(); break;
        case 19: _t->on_pushButton_line2_roi2_clicked(); break;
        case 20: _t->on_pushButton_line1_roi1_2_clicked(); break;
        case 21: _t->on_pushButton_line1_roi2_2_clicked(); break;
        case 22: _t->on_pushButton_line2_roi1_2_clicked(); break;
        case 23: _t->on_pushButton_line2_roi2_2_clicked(); break;
        case 24: _t->on_pushButton_StandardPosition_clicked(); break;
        case 25: _t->on_pushButton_camer1_Test_clicked(); break;
        case 26: _t->on_pushButton_camer2_Test_clicked(); break;
        case 27: _t->on_pushButton_calibration_rotationCenter_curren_clicked(); break;
        case 28: _t->on_pushButton_save_Json_clicked(); break;
        case 29: _t->on_pushButton_save_setting_clicked(); break;
        case 30: _t->on_pushButton_calibration_rotationCenter_offset_clicked(); break;
        case 31: _t->on_pushButton_calibration_translationX_curren_clicked(); break;
        case 32: _t->on_pushButton_calibration_translationX_offset_clicked(); break;
        case 33: _t->on_pushButton_calibration_translationY_curren_2_clicked(); break;
        case 34: _t->on_pushButton_calibration_translationY_offset_2_clicked(); break;
        case 35: _t->on_pushButton_run_clicked(); break;
        case 36: _t->camer1ImageSlot((*reinterpret_cast< cv::Mat(*)>(_a[1]))); break;
        case 37: _t->camer2ImageSlot((*reinterpret_cast< cv::Mat(*)>(_a[1]))); break;
        case 38: _t->currenAxisPointSlot((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5]))); break;
        case 39: _t->on_pushButton_control_moveZero_clicked(); break;
        case 40: _t->on_pushButton_moveP_negative_axisX_pressed(); break;
        case 41: _t->on_pushButton_moveP_negative_axisX_released(); break;
        case 42: _t->on_pushButton_moveP_positive_axisX_pressed(); break;
        case 43: _t->on_pushButton_moveP_positive_axisX_released(); break;
        case 44: _t->on_pushButton_moveP_negative_axisY_pressed(); break;
        case 45: _t->on_pushButton_moveP_negative_axisY_released(); break;
        case 46: _t->on_pushButton_moveP_positive_axisY_pressed(); break;
        case 47: _t->on_pushButton_moveP_positive_axisY_released(); break;
        case 48: _t->on_pushButton_moveP_negative_axisZ_pressed(); break;
        case 49: _t->on_pushButton_moveP_negative_axisZ_released(); break;
        case 50: _t->on_pushButton_moveP_positive_axisZ_pressed(); break;
        case 51: _t->on_pushButton_moveP_positive_axisZ_released(); break;
        case 52: _t->on_pushButton_moveP_negative_axisT_pressed(); break;
        case 53: _t->on_pushButton_moveP_negative_axisT_released(); break;
        case 54: _t->on_pushButton_moveP_positive_axisT_pressed(); break;
        case 55: _t->on_pushButton_moveP_positive_axisT_released(); break;
        case 56: _t->on_pushButton_moveP_negative_axisA_pressed(); break;
        case 57: _t->on_pushButton_moveP_negative_axisA_released(); break;
        case 58: _t->on_pushButton_moveP_positive_axisA_pressed(); break;
        case 59: _t->on_pushButton_moveP_positive_axisA_released(); break;
        case 60: _t->on_pushButton_control_record_load_clicked(); break;
        case 61: _t->on_pushButton_control_move_load_clicked(); break;
        case 62: _t->on_pushButton_control_record_platform_clicked(); break;
        case 63: _t->on_pushButton_control_move_platform_clicked(); break;
        case 64: _t->on_pushButton_control_record_detection_clicked(); break;
        case 65: _t->on_pushButton_control_move_detection_clicked(); break;
        case 66: _t->on_pushButton_control_record_blanking_clicked(); break;
        case 67: _t->on_pushButton_control_move_blanking_clicked(); break;
        case 68: _t->on_pushButton_control_enable_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 69: _t->on_checkBox_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 70: _t->on_pushButton_move_axisX_clicked(); break;
        case 71: _t->on_pushButton_control_proudctIO_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 72: _t->on_pushButton_control_platformIO_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 73: _t->on_pushButton_inProduct_clicked(); break;
        case 74: _t->on_pushButton_outProduct_clicked(); break;
        case 75: _t->on_pushButton_set_camer_ext_clicked(); break;
        case 76: _t->on_pushButton_set_camer_ext2_clicked(); break;
        case 77: _t->on_pushButton_move_axisY_clicked(); break;
        case 78: _t->on_pushButton_camerContinue_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 79: _t->on_pushButton_savePicture_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (MainWindow::*_t)(cv::Mat );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::camer1ImageSignal)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)(cv::Mat );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::camer2ImageSignal)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)(QString , QString , QString , QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::currenAxisPoint)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 80)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 80;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 80)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 80;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::camer1ImageSignal(cv::Mat _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainWindow::camer2ImageSignal(cv::Mat _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MainWindow::currenAxisPoint(QString _t1, QString _t2, QString _t3, QString _t4, QString _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
