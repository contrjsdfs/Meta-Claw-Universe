/****************************************************************************
** Meta object code from reading C++ file 'mlx90614_reader.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.10)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "mlx90614_reader.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mlx90614_reader.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.10. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MLX90614Reader_t {
    QByteArrayData data[11];
    char stringdata0[130];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MLX90614Reader_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MLX90614Reader_t qt_meta_stringdata_MLX90614Reader = {
    {
QT_MOC_LITERAL(0, 0, 14), // "MLX90614Reader"
QT_MOC_LITERAL(1, 15, 18), // "temperatureUpdated"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 10), // "objectTemp"
QT_MOC_LITERAL(4, 46, 11), // "ambientTemp"
QT_MOC_LITERAL(5, 58, 11), // "sensorError"
QT_MOC_LITERAL(6, 70, 7), // "message"
QT_MOC_LITERAL(7, 78, 12), // "startReading"
QT_MOC_LITERAL(8, 91, 10), // "intervalMs"
QT_MOC_LITERAL(9, 102, 11), // "stopReading"
QT_MOC_LITERAL(10, 114, 15) // "readTemperature"

    },
    "MLX90614Reader\0temperatureUpdated\0\0"
    "objectTemp\0ambientTemp\0sensorError\0"
    "message\0startReading\0intervalMs\0"
    "stopReading\0readTemperature"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MLX90614Reader[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   44,    2, 0x06 /* Public */,
       5,    1,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   52,    2, 0x0a /* Public */,
       7,    0,   55,    2, 0x2a /* Public | MethodCloned */,
       9,    0,   56,    2, 0x0a /* Public */,
      10,    0,   57,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Float, QMetaType::Float,    3,    4,
    QMetaType::Void, QMetaType::QString,    6,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MLX90614Reader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MLX90614Reader *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->temperatureUpdated((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 1: _t->sensorError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->startReading((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->startReading(); break;
        case 4: _t->stopReading(); break;
        case 5: _t->readTemperature(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MLX90614Reader::*)(float , float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MLX90614Reader::temperatureUpdated)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MLX90614Reader::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MLX90614Reader::sensorError)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MLX90614Reader::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_MLX90614Reader.data,
    qt_meta_data_MLX90614Reader,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MLX90614Reader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MLX90614Reader::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MLX90614Reader.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MLX90614Reader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void MLX90614Reader::temperatureUpdated(float _t1, float _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MLX90614Reader::sensorError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
