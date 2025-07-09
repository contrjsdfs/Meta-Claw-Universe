#ifndef MLX90614_READER_H
#define MLX90614_READER_H

#include <QObject>
#include <QTimer>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <cmath>

class MLX90614Reader : public QObject
{
    Q_OBJECT
public:
    explicit MLX90614Reader(QObject *parent = nullptr,
                          const QString &i2cDevice = "/dev/i2c-4",
                          quint8 slaveAddress = 0x5A);
    ~MLX90614Reader();

    bool initialize();
    float readObjectTemperature();
    float readAmbientTemperature();
    bool isSensorConnected();

signals:
    void temperatureUpdated(float objectTemp, float ambientTemp);
    void sensorError(const QString &message);

public slots:
    void startReading(int intervalMs = 1000);
    void stopReading();

private slots:
    void readTemperature();

private:
    int i2cFile;
    QString i2cDevice;
    quint8 slaveAddress;
    QTimer *readTimer;

    bool writeI2C(quint8 reg);
    quint16 readI2CWord(quint8 reg);
    float calculateTemperature(quint16 rawValue);
    quint8 calculatePEC(const quint8 *data, quint8 len);
};

#endif // MLX90614_READER_H
