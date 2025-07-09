#include "MLX90614.h"
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <chrono>
#include <thread>

// 如果I2C_M_RD仍未定义，手动定义
#ifndef I2C_M_RD
#define I2C_M_RD 1
#endif

MLX90614::MLX90614(QObject *parent) : QObject(parent) {}

bool MLX90614::init(const QString &i2cDevice)
{
    m_fd = open(i2cDevice.toUtf8().constData(), O_RDWR);
    if (m_fd < 0) {
        emit error(QString("Open I2C failed: %1").arg(strerror(errno)));
        return false;
    }

    if (ioctl(m_fd, I2C_SLAVE, ADDR) < 0) {
        emit error(QString("Set address failed: %1").arg(strerror(errno)));
        close(m_fd);
        m_fd = -1;
        return false;
    }

    // 设置超时和重试
    if (ioctl(m_fd, I2C_TIMEOUT, 1) < 0 || ioctl(m_fd, I2C_RETRIES, 3) < 0) {
        emit error("Warning: Failed to set I2C parameters");
    }

    return true;
}

bool MLX90614::readTemperature(float &temp)
{
    uint16_t raw;
    if (readRegister(RAM_ACCESS | RAM_TOBJ1, &raw)) {
        temp = raw * 0.02f - 273.15f;
        return true;
    }
    return false;
}

uint8_t MLX90614::calculatePEC(uint8_t addr, uint8_t cmd, const uint8_t *data)
{
    uint8_t buf[5] = { addr<<1, cmd, (addr<<1)|1, data[0], data[1] };
    uint8_t crc = 0;

    for (int i = 0; i < 5; ++i) {
        crc ^= buf[i];
        for (int j = 0; j < 8; ++j) {
            crc = (crc << 1) ^ ((crc & 0x80) ? 0x07 : 0);
        }
    }
    return crc;
}

bool MLX90614::readRegister(uint8_t reg, uint16_t *value)
{
    struct i2c_msg msgs[2] = {
        { ADDR, 0, 1, &reg },  // 写消息
        { ADDR, I2C_M_RD, 3, nullptr } // 读消息
    };

    uint8_t buf[3];
    msgs[1].buf = buf;

    struct i2c_rdwr_ioctl_data ioctl_data = {
        .msgs = msgs,
        .nmsgs = 2
    };

    for (int retry = 0; retry < 3; ++retry) {
        if (ioctl(m_fd, I2C_RDWR, &ioctl_data) >= 0) {
            uint8_t pec = calculatePEC(ADDR, reg, buf);
            if (buf[2] == pec) {
                *value = (buf[1] << 8) | buf[0];
                return true;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    emit error("Failed to read after retries");
    return false;
}
