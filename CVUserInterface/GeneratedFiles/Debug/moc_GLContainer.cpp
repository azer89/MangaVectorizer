/****************************************************************************
** Meta object code from reading C++ file 'GLContainer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stdafx.h"
#include "../../GLContainer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GLContainer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_GLContainer_t {
    QByteArrayData data[10];
    char stringdata[105];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_GLContainer_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_GLContainer_t qt_meta_stringdata_GLContainer = {
    {
QT_MOC_LITERAL(0, 0, 11),
QT_MOC_LITERAL(1, 12, 11),
QT_MOC_LITERAL(2, 24, 0),
QT_MOC_LITERAL(3, 25, 3),
QT_MOC_LITERAL(4, 29, 13),
QT_MOC_LITERAL(5, 43, 12),
QT_MOC_LITERAL(6, 56, 14),
QT_MOC_LITERAL(7, 71, 3),
QT_MOC_LITERAL(8, 75, 14),
QT_MOC_LITERAL(9, 90, 13)
    },
    "GLContainer\0SendMessage\0\0msg\0StrokeRelease\0"
    "UpdateThings\0VScrollChanged\0val\0"
    "HScrollChanged\0DummyFunction\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GLContainer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x05,
       4,    0,   47,    2, 0x05,
       5,    0,   48,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       6,    1,   49,    2, 0x08,
       8,    1,   52,    2, 0x08,
       9,    0,   55,    2, 0x08,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,

       0        // eod
};

void GLContainer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GLContainer *_t = static_cast<GLContainer *>(_o);
        switch (_id) {
        case 0: _t->SendMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->StrokeRelease(); break;
        case 2: _t->UpdateThings(); break;
        case 3: _t->VScrollChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->HScrollChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->DummyFunction(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (GLContainer::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GLContainer::SendMessage)) {
                *result = 0;
            }
        }
        {
            typedef void (GLContainer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GLContainer::StrokeRelease)) {
                *result = 1;
            }
        }
        {
            typedef void (GLContainer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GLContainer::UpdateThings)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject GLContainer::staticMetaObject = {
    { &QAbstractScrollArea::staticMetaObject, qt_meta_stringdata_GLContainer.data,
      qt_meta_data_GLContainer,  qt_static_metacall, 0, 0}
};


const QMetaObject *GLContainer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GLContainer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GLContainer.stringdata))
        return static_cast<void*>(const_cast< GLContainer*>(this));
    return QAbstractScrollArea::qt_metacast(_clname);
}

int GLContainer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractScrollArea::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void GLContainer::SendMessage(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GLContainer::StrokeRelease()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void GLContainer::UpdateThings()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
