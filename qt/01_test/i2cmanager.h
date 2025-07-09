#ifndef I2CMANAGER_H
#define I2CMANAGER_H

#include <QObject>
#include <QTimer>
#include <fcntl.h>          // 添加open()所需
#include <unistd.h>         //
#include <linux/i2c.h>  // For I2C_M_RD and other I2C macros
class I2CManager : public QObject
{
    Q_OBJECT
public:
    explicit I2CManager(QObject *parent = nullptr);
    ~I2CManager();

    bool init(const QString &device = "/dev/i2c-4", uint8_t addr = 0x34);
    void speak(uint8_t cmd, uint8_t idNum);
    int getResult();

signals:
    void commandReceived(int cmd, const QString &text);
    void errorOccurred(const QString &msg);

private slots:
    void pollI2C();

private:
    int m_fd = -1;
    uint8_t m_address;
    QTimer m_pollTimer;

    int i2cTransfer(struct i2c_msg *msgs, int num);
};

#endif // I2CMANAGER_H
