#include "mlx90614_reader.h"
#include <QDebug>

MLX90614Reader::MLX90614Reader(QObject *parent, const QString &i2cDevice, quint8 slaveAddress)
    : QObject(parent), i2cDevice(i2cDevice), slaveAddress(slaveAddress),
      i2cFile(-1), readTimer(nullptr)
{
}

MLX90614Reader::~MLX90614Reader()
{
    stopReading();
    if (i2cFile >= 0) {
        close(i2cFile);
    }
}

bool MLX90614Reader::initialize()
{
    // 1. 检查设备文件是否存在
    if(access(i2cDevice.toUtf8().constData(), F_OK) != 0) {
        qCritical() << "I2C设备不存在:" << i2cDevice;
        emit sensorError("I2C设备不存在");
        return false;
    }

    // 2. 尝试打开设备
    i2cFile = open(i2cDevice.toUtf8().constData(), O_RDWR);
    if(i2cFile < 0) {
        qCritical() << "无法打开I2C设备:" << strerror(errno);
        emit sensorError(QString("无法打开I2C设备: %1").arg(strerror(errno)));
        return false;
    }

    // 3. 设置从机地址
    if(ioctl(i2cFile, I2C_SLAVE, slaveAddress) < 0) {
        qCritical() << "设置I2C地址失败:" << strerror(errno);
        close(i2cFile);
        i2cFile = -1;
        emit sensorError(QString("设置I2C地址失败: %1").arg(strerror(errno)));
        return false;
    }

    // 4. 测试设备通信
    quint8 testReg = 0x00; // 尝试读取设备ID寄存器
    if(write(i2cFile, &testReg, 1) != 1) {
        qCritical() << "写入测试寄存器失败";
        close(i2cFile);
        i2cFile = -1;
        emit sensorError("写入测试寄存器失败");
        return false;
    }

    quint8 data[3];
    if(read(i2cFile, data, 3) != 3) {
        qCritical() << "读取测试数据失败";
        close(i2cFile);
        i2cFile = -1;
        emit sensorError("读取测试数据失败");
        return false;
    }

    qDebug() << "MLX90614初始化成功";
    return true;
}

bool MLX90614Reader::isSensorConnected()
{
    quint16 deviceId = readI2CWord(0x3C); // 读取设备ID寄存器
    return (deviceId != 0 && deviceId != 0xFFFF);
}

void MLX90614Reader::startReading(int intervalMs)
{
    if (!readTimer) {
        readTimer = new QTimer(this);
        connect(readTimer, &QTimer::timeout, this, &MLX90614Reader::readTemperature);
    }
    readTimer->start(intervalMs);
}

void MLX90614Reader::stopReading()
{
    if (readTimer) {
        readTimer->stop();
    }
}

void MLX90614Reader::readTemperature()
{
    float objectTemp = readObjectTemperature();
    float ambientTemp = readAmbientTemperature();

    qDebug() << "Raw data - Object:" << readI2CWord(0x07) << "Ambient:" << readI2CWord(0x06);
    qDebug() << "Calculated - Object:" << objectTemp << "Ambient:" << ambientTemp;

    if (!std::isnan(objectTemp) && !std::isnan(ambientTemp)) {
        emit temperatureUpdated(objectTemp, ambientTemp);
    } else {
        emit sensorError(tr("Invalid temperature reading"));
    }
}

float MLX90614Reader::readObjectTemperature()
{
    quint16 rawValue = readI2CWord(0x07); // RAM address for object temperature
    return calculateTemperature(rawValue);
}

float MLX90614Reader::readAmbientTemperature()
{
    quint16 rawValue = readI2CWord(0x06); // RAM address for ambient temperature
    return calculateTemperature(rawValue);
}

bool MLX90614Reader::writeI2C(quint8 reg)
{
    if (i2cFile < 0) return false;

    if (write(i2cFile, &reg, 1) != 1) {
        emit sensorError(tr("Failed to write to I2C device"));
        return false;
    }
    return true;
}

quint16 MLX90614Reader::readI2CWord(quint8 reg)
{
    if (i2cFile < 0) return 0;

    if (!writeI2C(reg)) {
        return 0;
    }

    quint8 data[3] = {0};
    if (read(i2cFile, data, 3) != 3) {
        emit sensorError(tr("Failed to read from I2C device"));
        return 0;
    }

    // 验证PEC
    quint8 calculatedPEC = calculatePEC(data, 2);
    if (calculatedPEC != data[2]) {
        emit sensorError(tr("PEC check failed (expected: 0x%1, got: 0x%2)")
                       .arg(calculatedPEC, 2, 16, QLatin1Char('0'))
                       .arg(data[2], 2, 16, QLatin1Char('0')));
        return 0;
    }

    // MLX90614使用小端模式
    quint16 value = (data[1] << 8) | data[0];
    return value;
}

float MLX90614Reader::calculateTemperature(quint16 rawValue)
{
    if (rawValue == 0 || rawValue == 0xFFFF) {
        return NAN;
    }

    float temperature = static_cast<float>(rawValue) * 0.02f - 273.15f;

    // 检查温度范围是否合理
    if (temperature < -40.0f || temperature > 380.0f) {
        qWarning() << "Temperature out of range:" << temperature << "(raw:" << rawValue << ")";
        return NAN;
    }

    return temperature;
}

quint8 MLX90614Reader::calculatePEC(const quint8 *data, quint8 len)
{
    quint8 crc = 0;
    for (quint8 i = 0; i < len; i++) {
        crc ^= data[i];
        for (quint8 j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ 0x07;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}
