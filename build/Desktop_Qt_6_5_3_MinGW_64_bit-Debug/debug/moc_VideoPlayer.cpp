/****************************************************************************
** Meta object code from reading C++ file 'VideoPlayer.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../VideoPlayer.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VideoPlayer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSvideoPlayerENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSvideoPlayerENDCLASS = QtMocHelpers::stringData(
    "videoPlayer",
    "on_pushButton_clicked",
    "",
    "on_PlayPause_clicked",
    "on_volumeSlider_sliderMoved",
    "position",
    "on_ProgressSlider_sliderMoved",
    "update_Frames",
    "frame",
    "on_openButton_clicked",
    "update_progress",
    "progress",
    "startDecode",
    "on_ProgressSlider_sliderPressed",
    "on_ProgressSlider_sliderReleased",
    "show_progress"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSvideoPlayerENDCLASS_t {
    uint offsetsAndSizes[32];
    char stringdata0[12];
    char stringdata1[22];
    char stringdata2[1];
    char stringdata3[21];
    char stringdata4[28];
    char stringdata5[9];
    char stringdata6[30];
    char stringdata7[14];
    char stringdata8[6];
    char stringdata9[22];
    char stringdata10[16];
    char stringdata11[9];
    char stringdata12[12];
    char stringdata13[32];
    char stringdata14[33];
    char stringdata15[14];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSvideoPlayerENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSvideoPlayerENDCLASS_t qt_meta_stringdata_CLASSvideoPlayerENDCLASS = {
    {
        QT_MOC_LITERAL(0, 11),  // "videoPlayer"
        QT_MOC_LITERAL(12, 21),  // "on_pushButton_clicked"
        QT_MOC_LITERAL(34, 0),  // ""
        QT_MOC_LITERAL(35, 20),  // "on_PlayPause_clicked"
        QT_MOC_LITERAL(56, 27),  // "on_volumeSlider_sliderMoved"
        QT_MOC_LITERAL(84, 8),  // "position"
        QT_MOC_LITERAL(93, 29),  // "on_ProgressSlider_sliderMoved"
        QT_MOC_LITERAL(123, 13),  // "update_Frames"
        QT_MOC_LITERAL(137, 5),  // "frame"
        QT_MOC_LITERAL(143, 21),  // "on_openButton_clicked"
        QT_MOC_LITERAL(165, 15),  // "update_progress"
        QT_MOC_LITERAL(181, 8),  // "progress"
        QT_MOC_LITERAL(190, 11),  // "startDecode"
        QT_MOC_LITERAL(202, 31),  // "on_ProgressSlider_sliderPressed"
        QT_MOC_LITERAL(234, 32),  // "on_ProgressSlider_sliderReleased"
        QT_MOC_LITERAL(267, 13)   // "show_progress"
    },
    "videoPlayer",
    "on_pushButton_clicked",
    "",
    "on_PlayPause_clicked",
    "on_volumeSlider_sliderMoved",
    "position",
    "on_ProgressSlider_sliderMoved",
    "update_Frames",
    "frame",
    "on_openButton_clicked",
    "update_progress",
    "progress",
    "startDecode",
    "on_ProgressSlider_sliderPressed",
    "on_ProgressSlider_sliderReleased",
    "show_progress"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSvideoPlayerENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   80,    2, 0x08,    1 /* Private */,
       3,    0,   81,    2, 0x08,    2 /* Private */,
       4,    1,   82,    2, 0x08,    3 /* Private */,
       6,    1,   85,    2, 0x08,    5 /* Private */,
       7,    1,   88,    2, 0x08,    7 /* Private */,
       9,    0,   91,    2, 0x08,    9 /* Private */,
      10,    1,   92,    2, 0x08,   10 /* Private */,
      12,    0,   95,    2, 0x08,   12 /* Private */,
      13,    0,   96,    2, 0x08,   13 /* Private */,
      14,    0,   97,    2, 0x08,   14 /* Private */,
      15,    1,   98,    2, 0x08,   15 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::QImage,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,   11,

       0        // eod
};

Q_CONSTINIT const QMetaObject videoPlayer::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSvideoPlayerENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSvideoPlayerENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSvideoPlayerENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<videoPlayer, std::true_type>,
        // method 'on_pushButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_PlayPause_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_volumeSlider_sliderMoved'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_ProgressSlider_sliderMoved'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'update_Frames'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QImage &, std::false_type>,
        // method 'on_openButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'update_progress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'startDecode'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_ProgressSlider_sliderPressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_ProgressSlider_sliderReleased'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'show_progress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>
    >,
    nullptr
} };

void videoPlayer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<videoPlayer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_pushButton_clicked(); break;
        case 1: _t->on_PlayPause_clicked(); break;
        case 2: _t->on_volumeSlider_sliderMoved((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->on_ProgressSlider_sliderMoved((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->update_Frames((*reinterpret_cast< std::add_pointer_t<QImage>>(_a[1]))); break;
        case 5: _t->on_openButton_clicked(); break;
        case 6: _t->update_progress((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 7: _t->startDecode(); break;
        case 8: _t->on_ProgressSlider_sliderPressed(); break;
        case 9: _t->on_ProgressSlider_sliderReleased(); break;
        case 10: _t->show_progress((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *videoPlayer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *videoPlayer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSvideoPlayerENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int videoPlayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
