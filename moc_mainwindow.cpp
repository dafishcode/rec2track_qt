/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "include/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[17];
    char stringdata0[440];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 13), // "updateBarrage"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 23), // "on_repeats_valueChanged"
QT_MOC_LITERAL(4, 50, 4), // "arg1"
QT_MOC_LITERAL(5, 55, 35), // "on_spinBox_inter_epoch_valueC..."
QT_MOC_LITERAL(6, 91, 30), // "on_btn_genBarrageFiles_clicked"
QT_MOC_LITERAL(7, 122, 37), // "on_btn_testVizStimOnProjector..."
QT_MOC_LITERAL(8, 160, 27), // "on_btn_CalcDuration_clicked"
QT_MOC_LITERAL(9, 188, 33), // "on_spinBox_framerate_valueCha..."
QT_MOC_LITERAL(10, 222, 36), // "on_spinBox_waiting_time_value..."
QT_MOC_LITERAL(11, 259, 34), // "on_btn_startVideoRecording_cl..."
QT_MOC_LITERAL(12, 294, 31), // "on_btnStartLiveTracking_clicked"
QT_MOC_LITERAL(13, 326, 26), // "on_btn_camLiveView_clicked"
QT_MOC_LITERAL(14, 353, 32), // "on_btn_selectStimSetFile_clicked"
QT_MOC_LITERAL(15, 386, 27), // "on_btn_selectfolder_clicked"
QT_MOC_LITERAL(16, 414, 25) // "on_radioVizStimOn_clicked"

    },
    "MainWindow\0updateBarrage\0\0"
    "on_repeats_valueChanged\0arg1\0"
    "on_spinBox_inter_epoch_valueChanged\0"
    "on_btn_genBarrageFiles_clicked\0"
    "on_btn_testVizStimOnProjector_clicked\0"
    "on_btn_CalcDuration_clicked\0"
    "on_spinBox_framerate_valueChanged\0"
    "on_spinBox_waiting_time_valueChanged\0"
    "on_btn_startVideoRecording_clicked\0"
    "on_btnStartLiveTracking_clicked\0"
    "on_btn_camLiveView_clicked\0"
    "on_btn_selectStimSetFile_clicked\0"
    "on_btn_selectfolder_clicked\0"
    "on_radioVizStimOn_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x08 /* Private */,
       3,    1,   85,    2, 0x08 /* Private */,
       5,    1,   88,    2, 0x08 /* Private */,
       6,    0,   91,    2, 0x08 /* Private */,
       7,    0,   92,    2, 0x08 /* Private */,
       8,    0,   93,    2, 0x08 /* Private */,
       9,    1,   94,    2, 0x08 /* Private */,
      10,    1,   97,    2, 0x08 /* Private */,
      11,    0,  100,    2, 0x08 /* Private */,
      12,    0,  101,    2, 0x08 /* Private */,
      13,    0,  102,    2, 0x08 /* Private */,
      14,    0,  103,    2, 0x08 /* Private */,
      15,    0,  104,    2, 0x08 /* Private */,
      16,    0,  105,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateBarrage(); break;
        case 1: _t->on_repeats_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_spinBox_inter_epoch_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_btn_genBarrageFiles_clicked(); break;
        case 4: _t->on_btn_testVizStimOnProjector_clicked(); break;
        case 5: _t->on_btn_CalcDuration_clicked(); break;
        case 6: _t->on_spinBox_framerate_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->on_spinBox_waiting_time_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->on_btn_startVideoRecording_clicked(); break;
        case 9: _t->on_btnStartLiveTracking_clicked(); break;
        case 10: _t->on_btn_camLiveView_clicked(); break;
        case 11: _t->on_btn_selectStimSetFile_clicked(); break;
        case 12: _t->on_btn_selectfolder_clicked(); break;
        case 13: _t->on_radioVizStimOn_clicked(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


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
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
