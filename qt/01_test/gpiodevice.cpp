// gpiodevice.cpp
#include "gpiodevice.h"
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <QDebug>

GpioDevice::GpioDevice(QObject *parent) : QObject(parent) {}

GpioDevice::~GpioDevice() {
    closeDevice();
}

bool GpioDevice::openDevice() {
    if (m_fd >= 0) return true;

    m_fd = open(DEVICE_PATH, O_RDWR);
    if (m_fd < 0) {
        qDebug() << "Open device failed:" << strerror(errno);
        return false;
    }
    return true;
}

void GpioDevice::closeDevice() {
    if (m_fd >= 0) {
        close(m_fd);
        m_fd = -1;
    }
}

bool GpioDevice::setPin(int pinNumber) {
    if (!openDevice()) return false;

    if (ioctl(m_fd, SET_GPIO_CMD, pinNumber) < 0) {
        qDebug() << "Set GPIO failed:" << strerror(errno);
        return false;
    }
    return true;
}

bool GpioDevice::writeValue(char value) {
    if (!openDevice()) return false;

    if (write(m_fd, &value, 1) != 1) {
        qDebug() << "Write GPIO failed:" << strerror(errno);
        return false;
    }
    return true;
}

char GpioDevice::readValue() {
    if (!openDevice()) return '\0';

    char value;
    if (read(m_fd, &value, 1) != 1) {
        qDebug() << "Read GPIO failed:" << strerror(errno);
        return '\0';
    }
    return value;
}
