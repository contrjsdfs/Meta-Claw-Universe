#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QtCore/QCoreApplication>
#include <linux/input.h>
#include <QSlider>
#include <uart_thread.h>
#include <QProcess>
#include "i2cmanager.h"
#include <QTcpSocket>    //添加的头文件
#include <TemperatureGauge.h>
#include <Thermometerwidget.h>
#include "gpiodevice.h"
#include "mlx90614_reader.h"
#include "MLX90614.h"
#include <QPushButton>
QT_BEGIN_NAMESPACE
extern  bool mode;  // 声明为extern
extern int foodweight;
extern bool ledstate;
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTimer* my_timer;
private slots:
    void on_pushButton_clicked(bool checked);
    void slot_timeout(void);
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked(bool checked);
    void on_pushButton_6_clicked(bool checked);
    void on_toggleButton_clicked();
    void onCommandReceived(int cmd, const QString &text);
    void ap3216_timeout();
public slots:
    void startConnection();
    void onConnected();
    void onDisconnected();
    void onSocketError(QAbstractSocket::SocketError error);
    void sendJsonData();
    void processReceivedData();
    void on_openBt_clicked();
    void on_closeBt_clicked();
    void on_playComfortButton_clicked(bool checked);  // 播放/停止 "comfort.mp3"
    void on_playSleepButton_clicked(bool checked);    // 播放/停止 "sleep.mp3"

private:
    std::atomic<bool> m_terminating{false};
    Ui::MainWindow *ui;
    I2CManager *m_i2c;
    QTcpSocket *tcpSocket;
    QTimer *sendTimer;
    GpioDevice *m_gpio;
    MLX90614Reader *mlx90614Reader;
    MLX90614 *m_sensor;
    QTimer *m_timer;

    int led_fd, beep_fd, ap3216_fd, dht11_fd,
        jdq_fd, sg90_fd,sg_fd, sr501_fd, bluetooth_fd,gpio_fd,fun_fd,hot_fd,wu_fd,dht_fd,hx711_fd,youren_fd,door_fd;
    int sr501_flags;
    int jdq_status = 0;

    QSlider *slider = new QSlider();

    QString led_dev = "/sys/class/leds/work/brightness";
    QString beep_dev = "/dev/beep";
    QString ap3216_dev = "/dev/ap3216";
    QString dht11_dev = "/dev/my_dht11";
    QString jdq_dev = "/dev/jdq";
    QString sg90_dev = "/dev/sg90";
    QString sr501_dev = "/dev/sr501";
    QString bluetooth_dev = "/dev/ttymxc2";
    QString gpio_dev = "/sys/class/gpio/gpio101/value"; // /sys/class/pwm/pwmchip0/pwm0#
    QString dht_dev = "/sys/class/gpio/gpio103/value";
    QString hx711_dev = "/dev/my_hx711";
    QString youren_dev = "/dev/hcsr04";
    QString fun_dev = "/sys/class/gpio/gpio104/value";
    QString wu_dev = "/sys/class/gpio/gpio123/value";
    QString hot_dev = "/sys/class/gpio/gpio114/value";
    FILE *raw_fd, *scale_fd,*wendu_fd, *shidu_fd,*guang_fd;
    const char *voltage1_raw = "/sys/bus/iio/devices/iio:device0/in_voltage6_raw";//空气
    const char *voltage_scale = "/sys/bus/iio/devices/iio:device0/in_voltage_scale";
    const char *voltageyouren_raw = "/sys/bus/iio/devices/iio:device0/in_voltage4_raw";//温度
    const char *voltageshidu_raw = "/sys/bus/iio/devices/iio:device0/in_voltage5_raw";//湿度
    const char *voltageguang_raw = "/sys/bus/iio/devices/iio:device0/in_voltage7_raw";//光照
    const char *dht_dev1 = "/sys/class/gpio/gpio103/value";

    unsigned char buf[10];

    QTimer *ap3216c_tim;

    TemperatureGauge *gauge;
    ThermometerWidget *thermo = new ThermometerWidget(this);
    QTimer *beep_tim;

    QMouseEvent *event;


/**********music*****************/
    QProcess *process;
    QProcess *m_comfortProcess = nullptr;
    QProcess *m_sleepProcess = nullptr;

    const QString m_comfortAudioPath = "/userdata/media/audio/comfort.mp3";
    const QString m_sleepAudioPath = "/userdata/media/audio/sleep.mp3";
    // 核心功能方法
    void stopAllPlayers();
    void cleanupPlayer(QProcess* &process, QPushButton *button);
    bool startPlayer(const QString &filePath, QProcess* &process, QPushButton *button);
    void updateButtonState(QPushButton *button, bool isPlaying);
    bool checkAudioFile(const QString &path);
    void setupGStreamerEnvironment(QProcess *process);

    bool isLedOn = false;
    void updateLedState();
    void updatafood();
    void updatawater();
    void updatadoor();
    void updateLEDs();
    void setGpioLow(int pinNumber);
    void setGpiohigh(int pinNumber);
    void turnOnLED();
    void turnOffLED();
    void openfood();
    void closefood();
    void onTemperatureUpdated(float objectTemp, float ambientTemp);  // void MainWindow::updateUI(double humidity, double temperature)；
    void onSensorError(const QString &message);
    void updateTemperature();
    void handleError(const QString &msg);
    static void sr501_handler(int signum);
    int open_port(const char *com, int nSpeed);
    QJsonObject createJsonObject();
    void setupConnections();

};
#endif // MAINWINDOW_H
