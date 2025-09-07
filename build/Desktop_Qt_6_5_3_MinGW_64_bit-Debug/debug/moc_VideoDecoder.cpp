/****************************************************************************
** Meta object code from reading C++ file 'VideoDecoder.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../VideoDecoder.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VideoDecoder.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSVideoDecoderENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSVideoDecoderENDCLASS = QtMocHelpers::stringData(
    "VideoDecoder",
    "frameReady",
    "",
    "w",
    "h",
    "const uint8_t*",
    "y",
    "yStride",
    "u",
    "uStride",
    "v",
    "vStride",
    "updateProgress",
    "time"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSVideoDecoderENDCLASS_t {
    uint offsetsAndSizes[28];
    char stringdata0[13];
    char stringdata1[11];
    char stringdata2[1];
    char stringdata3[2];
    char stringdata4[2];
    char stringdata5[15];
    char stringdata6[2];
    char stringdata7[8];
    char stringdata8[2];
    char stringdata9[8];
    char stringdata10[2];
    char stringdata11[8];
    char stringdata12[15];
    char stringdata13[5];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSVideoDecoderENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSVideoDecoderENDCLASS_t qt_meta_stringdata_CLASSVideoDecoderENDCLASS = {
    {
        QT_MOC_LITERAL(0, 12),  // "VideoDecoder"
        QT_MOC_LITERAL(13, 10),  // "frameReady"
        QT_MOC_LITERAL(24, 0),  // ""
        QT_MOC_LITERAL(25, 1),  // "w"
        QT_MOC_LITERAL(27, 1),  // "h"
        QT_MOC_LITERAL(29, 14),  // "const uint8_t*"
        QT_MOC_LITERAL(44, 1),  // "y"
        QT_MOC_LITERAL(46, 7),  // "yStride"
        QT_MOC_LITERAL(54, 1),  // "u"
        QT_MOC_LITERAL(56, 7),  // "uStride"
        QT_MOC_LITERAL(64, 1),  // "v"
        QT_MOC_LITERAL(66, 7),  // "vStride"
        QT_MOC_LITERAL(74, 14),  // "updateProgress"
        QT_MOC_LITERAL(89, 4)   // "time"
    },
    "VideoDecoder",
    "frameReady",
    "",
    "w",
    "h",
    "const uint8_t*",
    "y",
    "yStride",
    "u",
    "uStride",
    "v",
    "vStride",
    "updateProgress",
    "time"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSVideoDecoderENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    8,   26,    2, 0x06,    1 /* Public */,
      12,    1,   43,    2, 0x06,   10 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, 0x80000000 | 5, QMetaType::Int, 0x80000000 | 5, QMetaType::Int, 0x80000000 | 5, QMetaType::Int,    3,    4,    6,    7,    8,    9,   10,   11,
    QMetaType::Void, QMetaType::Double,   13,

       0        // eod
};

Q_CONSTINIT const QMetaObject VideoDecoder::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSVideoDecoderENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSVideoDecoderENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSVideoDecoderENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<VideoDecoder, std::true_type>,
        // method 'frameReady'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<const uint8_t *, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<const uint8_t *, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<const uint8_t *, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'updateProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>
    >,
    nullptr
} };

void VideoDecoder::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VideoDecoder *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->frameReady((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<const uint8_t*>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<const uint8_t*>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[6])),(*reinterpret_cast< std::add_pointer_t<const uint8_t*>>(_a[7])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[8]))); break;
        case 1: _t->updateProgress((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (VideoDecoder::*)(int , int , const uint8_t * , int , const uint8_t * , int , const uint8_t * , int );
            if (_t _q_method = &VideoDecoder::frameReady; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (VideoDecoder::*)(double );
            if (_t _q_method = &VideoDecoder::updateProgress; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject *VideoDecoder::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoDecoder::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSVideoDecoderENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int VideoDecoder::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void VideoDecoder::frameReady(int _t1, int _t2, const uint8_t * _t3, int _t4, const uint8_t * _t5, int _t6, const uint8_t * _t7, int _t8)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t5))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t6))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t7))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t8))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void VideoDecoder::updateProgress(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
