#include "i2cmanager.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <QDebug>
#include <linux/i2c.h>      // For I2C_M_RD, i2c_msg, i2c_rdwr_ioctl_data
#include <linux/i2c-dev.h>  // For I2C_RDWR
#include <sys/ioctl.h>      // For ioctl()
I2CManager::I2CManager(QObject *parent) : QObject(parent)
{
    connect(&m_pollTimer, &QTimer::timeout, this, &I2CManager::pollI2C);
}

bool I2CManager::init(const QString &device, uint8_t addr)
{
    m_fd = open(device.toUtf8(), O_RDWR);
    if(m_fd < 0) {
        emit errorOccurred(QString("打开I2C设备失败: %1").arg(strerror(errno)));
        return false;
    }

    m_address = addr;
    m_pollTimer.start(100); // 每100ms轮询一次
    return true;
}

int I2CManager::i2cTransfer(struct i2c_msg *msgs, int num)
{
    struct i2c_rdwr_ioctl_data packets = {
        .msgs = msgs,
        .nmsgs = (unsigned int)num
    };
    return ioctl(m_fd, I2C_RDWR, &packets);
}

void I2CManager::pollI2C()
{
    uint8_t reg = 0x64; // ASR结果寄存器
    uint8_t result = 0;

    struct i2c_msg msgs[2] = {
        { // 写寄存器地址
            .addr = m_address,
            .flags = 0,
            .len = 1,
            .buf = &reg
        },
        { // 读数据
            .addr = m_address,
            .flags = I2C_M_RD,
            .len = 1,
            .buf = &result
        }
    };

    if(i2cTransfer(msgs, 2) >= 0 && result > 0) {
        QString cmdText;
        switch(result) {
            case 0x01: cmdText = "前进"; break;
            case 0x02: cmdText = "后退"; break;
            case 0x03: cmdText = "左转"; break;
            case 0x04: cmdText = "右转"; break;
            case 0x09: cmdText = "停止"; break;
            default: cmdText = "未知指令";
        }
        emit commandReceived(result, cmdText);
    }
}

void I2CManager::speak(uint8_t cmd, uint8_t idNum)
{
    uint8_t data[3] = {0x6E, cmd, idNum}; // 播报寄存器地址+命令+ID

    struct i2c_msg msg = {
        .addr = m_address,
        .flags = 0,
        .len = sizeof(data),
        .buf = data
    };

    if(i2cTransfer(&msg, 1) < 0) {
        emit errorOccurred(QString("播报失败: %1").arg(strerror(errno)));
    }
    usleep(20000); // 20ms延迟
}

I2CManager::~I2CManager()
{
    if(m_fd >= 0) close(m_fd);
}
