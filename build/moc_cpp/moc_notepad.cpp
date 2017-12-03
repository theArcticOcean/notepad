/****************************************************************************
** Meta object code from reading C++ file 'notepad.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Inc/notepad.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'notepad.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_notePad_t {
    QByteArrayData data[24];
    char stringdata[361];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_notePad_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_notePad_t qt_meta_stringdata_notePad = {
    {
QT_MOC_LITERAL(0, 0, 7),
QT_MOC_LITERAL(1, 8, 14),
QT_MOC_LITERAL(2, 23, 0),
QT_MOC_LITERAL(3, 24, 3),
QT_MOC_LITERAL(4, 28, 13),
QT_MOC_LITERAL(5, 42, 13),
QT_MOC_LITERAL(6, 56, 9),
QT_MOC_LITERAL(7, 66, 5),
QT_MOC_LITERAL(8, 72, 19),
QT_MOC_LITERAL(9, 92, 21),
QT_MOC_LITERAL(10, 114, 20),
QT_MOC_LITERAL(11, 135, 19),
QT_MOC_LITERAL(12, 155, 20),
QT_MOC_LITERAL(13, 176, 20),
QT_MOC_LITERAL(14, 197, 20),
QT_MOC_LITERAL(15, 218, 20),
QT_MOC_LITERAL(16, 239, 23),
QT_MOC_LITERAL(17, 263, 20),
QT_MOC_LITERAL(18, 284, 23),
QT_MOC_LITERAL(19, 308, 14),
QT_MOC_LITERAL(20, 323, 6),
QT_MOC_LITERAL(21, 330, 10),
QT_MOC_LITERAL(22, 341, 12),
QT_MOC_LITERAL(23, 354, 5)
    },
    "notePad\0changeFileName\0\0str\0actionEncrypt\0"
    "actionDecrypt\0deleteTab\0index\0"
    "actionNew_triggered\0actionPaste_triggered\0"
    "actionCopy_triggered\0actionCut_triggered\0"
    "actionRedo_triggered\0actionUndo_triggered\0"
    "actionOpen_triggered\0actionSave_triggered\0"
    "actionSave_as_triggered\0actionFind_triggered\0"
    "actionReplace_triggered\0actionInitFont\0"
    "backup\0closeEvent\0QCloseEvent*\0event\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_notePad[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  104,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       4,    0,  107,    2, 0x0a,
       5,    0,  108,    2, 0x0a,
       6,    1,  109,    2, 0x0a,
       8,    0,  112,    2, 0x0a,
       9,    0,  113,    2, 0x0a,
      10,    0,  114,    2, 0x0a,
      11,    0,  115,    2, 0x0a,
      12,    0,  116,    2, 0x0a,
      13,    0,  117,    2, 0x0a,
      14,    0,  118,    2, 0x0a,
      15,    0,  119,    2, 0x0a,
      16,    0,  120,    2, 0x0a,
      17,    0,  121,    2, 0x0a,
      18,    0,  122,    2, 0x0a,
      19,    0,  123,    2, 0x0a,
      20,    0,  124,    2, 0x0a,
      21,    1,  125,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
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
    QMetaType::Void, 0x80000000 | 22,   23,

       0        // eod
};

void notePad::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        notePad *_t = static_cast<notePad *>(_o);
        switch (_id) {
        case 0: _t->changeFileName((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->actionEncrypt(); break;
        case 2: _t->actionDecrypt(); break;
        case 3: _t->deleteTab((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->actionNew_triggered(); break;
        case 5: _t->actionPaste_triggered(); break;
        case 6: _t->actionCopy_triggered(); break;
        case 7: _t->actionCut_triggered(); break;
        case 8: _t->actionRedo_triggered(); break;
        case 9: _t->actionUndo_triggered(); break;
        case 10: _t->actionOpen_triggered(); break;
        case 11: _t->actionSave_triggered(); break;
        case 12: _t->actionSave_as_triggered(); break;
        case 13: _t->actionFind_triggered(); break;
        case 14: _t->actionReplace_triggered(); break;
        case 15: _t->actionInitFont(); break;
        case 16: _t->backup(); break;
        case 17: _t->closeEvent((*reinterpret_cast< QCloseEvent*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (notePad::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&notePad::changeFileName)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject notePad::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_notePad.data,
      qt_meta_data_notePad,  qt_static_metacall, 0, 0}
};


const QMetaObject *notePad::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *notePad::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_notePad.stringdata))
        return static_cast<void*>(const_cast< notePad*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int notePad::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void notePad::changeFileName(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
