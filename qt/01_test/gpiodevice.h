// gpiodevice.h
#ifndef GPIODEVICE_H  // 添加这行
#define GPIODEVICE_H  // 添加这行

#include <QObject>
#include <QFile>

class GpioDevice : public QObject {
    Q_OBJECT
public:
    explicit GpioDevice(QObject *parent = nullptr);
    ~GpioDevice();

    bool openDevice();
    void closeDevice();
    bool setPin(int pinNumber);
    bool writeValue(char value);
    char readValue();

private:
    int m_fd = -1;
    static constexpr const char* DEVICE_PATH = "/dev/mygpio";
    static constexpr int SET_GPIO_CMD = 0;
};

#endif // GPIODEVICE_H  // 添加这行
