#ifndef MLX90614_H
#define MLX90614_H

#include <QObject>
#include <QFile>

class MLX90614 : public QObject
{
    Q_OBJECT
public:
    explicit MLX90614(QObject *parent = nullptr);
    bool init(const QString &i2cDevice = "/dev/i2c-7");
    bool readTemperature(float &temp);

signals:
    void error(const QString &msg);

private:
    uint8_t calculatePEC(uint8_t addr, uint8_t cmd, const uint8_t *data);
    bool readRegister(uint8_t reg, uint16_t *value);

    int m_fd = -1;
    static constexpr uint8_t ADDR = 0x5A;
    static constexpr uint8_t RAM_ACCESS = 0x00;
    static constexpr uint8_t RAM_TOBJ1 = 0x07;
};

#endif // MLX90614_H
