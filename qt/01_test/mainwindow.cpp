#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/signal.h>
#include <QDebug>
#include <QMouseEvent>
#include <thread>
#include <errno.h>
#include <termios.h>
#include <stdlib.h>
#include <QTimer>
#include <QTime>
#include <QDateTime>
#include <QListView>
#include <TemperatureGauge.h>
#include <Thermometerwidget.h>
#include <heart.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QStatusBar>   // 添加这行
#include "gpiodevice.h"
#include <QSettings>
#include <QMessageBox>
#include "MLX90614.h"
//control gpio
#define SYSFS_GPIO_EXPORT           "/sys/class/gpio/export"
#define SYSFS_GPIO_RST_PIN_VAL      "101"
#define DHT11_PIN_VAL               "117"
#define DHT11_PIN_VAL_DIR          "/sys/class/gpio/gpio117/direction"
#define SYSFS_GPIO_RST_DIR          "/sys/class/gpio/gpio101/direction"
#define SYSFS_GPIO_RST_DIR_VAL      "out"
#define SYSFS_GPIO_IN_DIR_VAL       "in"
//none
#define TempH   32
#define RHH     80
#define TempL   25
#define RHL     20
// sensor
#define Fan     104
#define Wu      123
#define Hot     114
#define Cold    115
#define WaterPin  97
#define LED 101
// door state
#define dooropen "2000000"
#define doorclose "600000"
//food state
#define foodopen "2100000"
#define foodclose "600000"
//sg90--food
#define sg90_export                 "/sys/class/pwm/pwmchip0/export"
#define sg90_period                 "/sys/class/pwm/pwmchip0/pwm0/period"
#define sg90_duty                   "/sys/class/pwm/pwmchip0/pwm0/duty_cycle"
#define sg90_polarity               "/sys/class/pwm/pwmchip0/pwm0/polarity"
#define sg90_able                   "/sys/class/pwm/pwmchip0/pwm0/enable"
//door--door
#define door_export                 "/sys/class/pwm/pwmchip1/export"
#define door_period                 "/sys/class/pwm/pwmchip1/pwm0/period"
#define door_duty                   "/sys/class/pwm/pwmchip1/pwm0/duty_cycle"
#define door_polarity               "/sys/class/pwm/pwmchip1/pwm0/polarity"
#define door_able                   "/sys/class/pwm/pwmchip1/pwm0/enable"
//hx711 --standard
#define WEIGHTS_REF 158.0f    // 参考重量(g)
#define WEIGHTS_ADC 8262799
// 参考重量对应的ADC值
int32_t value, reset_value = 0;//weight reset
int first_reading = 1;  //weight first
/* 给静态成员函数：sr501_handler 提供全局变量
   否则无法访问到类中的非静态成员变量*/
int WCNUm=0;
int WCFlag=0,WCFlag_last=0;
float shiduxianshi=0,wenduxianshi=29.8;
float temper,humidity,sun,weightsend,watersend=0;
bool mode = false;// auto/hand
float weight=0;
float temph=30,templ=23,tiwen;
int humh = 70,huml = 30,foodweight=5,xiaodutime=5;
bool m_terminating = false;
static bool water = true;
bool ledstate = false;
Ui::MainWindow *temp_ui;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_gpio(new GpioDevice(this))
    , m_i2c(new I2CManager(this))
    , m_sensor(new MLX90614(this))
{
    ui->setupUi(this);
    temp_ui = ui;
    
    /***************mlx90614--i2c--体温*********************************/
    setWindowFlags(Qt::FramelessWindowHint);
    // 初始化传感器
    if (!m_sensor->init()) {
        QMessageBox::critical(this, "错误", "传感器初始化失败!");
        ui->tempEdit->setEnabled(false);
        return;
    }
    // 连接信号槽
    connect(m_sensor, &MLX90614::error, this, &MainWindow::handleError);
    
    /******************temperature/hum label control*********************/
    updateTemperature();
    // 设置定时器
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::updateTemperature);
    m_timer->start(1000); // 每秒更新
    TemperatureGauge *gauge = new TemperatureGauge(this);
    gauge->setSizeMM(15, 80);  // 总宽度15mm(足够容纳9mm柱状图)，总高度80mm
    gauge->setRange(0, 100);
    gauge->setTemperature(75.5);
    ui->verticalLayout->insertWidget(0, gauge);
    ThermometerWidget *thermo = new ThermometerWidget(this);
    thermo->setSizeMM(20, 70);  // 15mm宽 100mm高
    thermo->setRange(-20, 50);   // 量程-20~50°C
    thermo->setTemperature(25);  // 初始值
    // 添加到布局
    ui->centralLayout->addWidget(thermo, 0, Qt::AlignHCenter);
    // 模拟温度变化
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [thermo,gauge](){
    thermo->setTemperature(wenduxianshi);
    gauge->setTemperature(shiduxianshi);
    });
    timer->start(1000);

    /***************************music**********************************/
    ui->playComfortButton->setCheckable(true);
    ui->playSleepButton->setCheckable(true);
    updateButtonState(ui->playComfortButton, false);
    updateButtonState(ui->playSleepButton, false);

    /***********************TCP/IP************************************/
    process = new QProcess(this);
    tcpSocket= new QTcpSocket(this);
    sendTimer = nullptr;  // 初始化为nullptr
    connect(tcpSocket, &QTcpSocket::connected, this, &MainWindow::onConnected);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &MainWindow::onDisconnected);
    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this, &MainWindow::onSocketError);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &MainWindow::processReceivedData);
    startConnection();
    
    /**********************Voice Assistant--II2**************************/
   // 初始化I2C
   if(!m_i2c->init()) {
       QMessageBox::critical(this, "错误", "I2C初始化失败");
       close();
   }
   // 连接信号槽
   connect(m_i2c, &I2CManager::commandReceived, this, &MainWindow::onCommandReceived);
   connect(m_i2c, &I2CManager::errorOccurred, [this](const QString &msg){
       ui->textEdit->setText("[错误WCFlag] " + msg);
   });
    /*******************************启动定时器*****************************/
   my_timer =  new QTimer(this);
   connect(my_timer,SIGNAL(timeout()),this,SLOT(slot_timeout()));
   my_timer->start(1000);

   /**********************************GET TIME*****************************/
       ui->label_time_1->setText(QTime::currentTime().toString("hh:mm"));
       ui->label_time_2->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd ddd"));
       
    /**********************************open water pump**********************/
    gpio_fd = open(SYSFS_GPIO_EXPORT, O_WRONLY);
    if(gpio_fd < 0)
        printf("open sb failed\n");
     write(gpio_fd, SYSFS_GPIO_RST_PIN_VAL ,sizeof(SYSFS_GPIO_RST_PIN_VAL));
    
     gpio_fd = open(SYSFS_GPIO_RST_DIR, O_WRONLY);
     if(gpio_fd < 0)
         printf("open sb failed\n");
     write(gpio_fd, SYSFS_GPIO_RST_DIR_VAL, sizeof(SYSFS_GPIO_RST_DIR_VAL));
    
     gpio_fd = open(gpio_dev.toStdString().c_str(), O_RDWR);
     if(gpio_fd < 0)
         printf("open sb failed\n");

    /************************************food******************************/
    sg_fd = open(sg90_export, O_WRONLY);
    if(sg_fd < 0)
     printf("open sg failed\n");
    write(sg_fd, "0" ,sizeof("0"));
    sg_fd = open(sg90_polarity, O_WRONLY);
    if(sg_fd < 0)
      printf("open sg1 failed\n");
    write(sg_fd, "normal", sizeof("normal"));
    sg_fd = open(sg90_period, O_WRONLY);
    if(sg_fd < 0)
      printf("open sg2 failed\n");
    write(sg_fd, "20000000", sizeof("20000000"));
    sg_fd = open(sg90_duty, O_WRONLY);
    if(sg_fd < 0)
      printf("open sg3 failed\n");
    write(sg_fd, foodclose, sizeof(foodclose));
    sg_fd = open(sg90_able, O_WRONLY);
    if(sg_fd < 0)
      printf("open sg4 failed\n");
    write(sg_fd, "1", sizeof("1"));
    sg_fd = open(sg90_duty, O_WRONLY);
    if(sg_fd < 0)
      printf("open sg56 failed\n");
    
    /**********************************door****************************/
    door_fd = open(door_export, O_WRONLY);
    if(door_fd < 0)
      printf("open sg failed\n");
    write(door_fd, "0" ,sizeof("0"));
    door_fd = open(door_polarity, O_WRONLY);
    if(door_fd < 0)
       printf("open sg1 failed\n");
    write(door_fd, "normal", sizeof("normal"));
    door_fd = open(door_period, O_WRONLY);
    if(door_fd < 0)
       printf("open sg2 failed\n");
    write(door_fd, "20000000", sizeof("20000000"));
    door_fd = open(door_duty, O_WRONLY);
    if(door_fd < 0)
       printf("open sg3 failed\n");
    write(door_fd, doorclose, sizeof(doorclose));
    door_fd = open(door_able, O_WRONLY);
    if(door_fd < 0)
       printf("open sg4 failed\n");
    write(door_fd, "1", sizeof("1"));
    door_fd = open(door_duty, O_WRONLY);
    if(door_fd < 0)
       printf("open door failed\n");
    
    /****************************open weight*********************************/
    hx711_fd = open(hx711_dev.toStdString().c_str(), O_RDWR);
    if(hx711_fd < 0)
      printf("open hx711 failed\n");
    
    /*****************************open hcsr04*******************************/
    youren_fd = open(youren_dev.toStdString().c_str(), O_RDONLY);
    if(youren_fd < 0)
      printf("open hw failed\n");
    
    /****************************open dht11*********************************/
    dht11_fd = open(dht11_dev.toStdString().c_str(), O_RDONLY);
    if(dht11_fd < 0)
        printf("open dht11 failed\n");
}

/********************* timeout read water and dht11 and temp  hum  weight ********************************/
void MainWindow::ap3216_timeout()
{
    //init
    unsigned short ir, light, dis;
    static int cnt = 0;
    int raw = 300;// 添加这行声明
    int guang,wendu,youren,shidu=300 ;
    char data[20] = {0};
    char dataguang[20] = {0};
    char datawendu[20] = {0};
    char datashidu[20] = {0};
    char datayouren[20] = {0};
    int fd;
    int ret; 

    /*********************1.5秒读一次****************************/ 
    if(cnt % 3 == 0){
        /****************打开water文件***************************/
        raw_fd = fopen(voltage1_raw, "r");

        if(raw_fd == NULL){// 添加这行声明
            printf("open raw_fd failed!\n");
        }else{
            rewind(raw_fd);   // 将光标移回文件开头
            fscanf(raw_fd, "%s", data);
            raw = atoi(data);
            //一定要关闭文件，否则占用文件后内核无法更新ADC值
            fclose(raw_fd);
        }
        watersend = raw;
        char light_str[32],water_str[32];
        snprintf(water_str, sizeof(water_str), "%dmm", raw);
        ui->label_air->setText(water_str);
         
        /*******************open sun file*******************/
        guang_fd = fopen(voltageguang_raw, "r");
        if(guang_fd == NULL){
            printf("open guang_fd failed!\n");
        }else{
            rewind(guang_fd);   // 将光标移回文件开头
            fscanf(guang_fd, "%s", dataguang);
            guang = atoi(dataguang);
            //一定要关闭文件，否则占用文件后内核无法更新ADC值
            fclose(guang_fd);
        }
        sun = 100-(guang*100/4096);
        snprintf(light_str, sizeof(light_str), "%d%%", 100-(guang*100/4096));
        ui->label_light->setText(light_str); 
        /******************************weight*******************/
          ret = read(hx711_fd, &value, sizeof(value));
          // 如果是第一次读取，将其作为reset值
          if (first_reading) {
              reset_value = value;
              first_reading = 0;
          }
          // 计算重量（根据提供的公式）
          weight = (float)(value - reset_value) * WEIGHTS_REF / (float)(WEIGHTS_ADC - reset_value);
          if(weight<0)weight=0;
          snprintf(light_str, sizeof(light_str), "%.2f", weight);
          ui->label_dis->setText(light_str);
      }
     

    /******************** 3秒读取一次dht11 **********************************/
    if (cnt % 5 == 0) {
        cnt = 0;
        /* 读取DHT11传感器数据 */
        unsigned char buf1[4];
        int ret = read(dht11_fd, buf1, 4);
        if (ret == 4) {
            char tmp_str[32];
            char hum_str[32];
            // 格式化温度字符串（整数部分.小数部分）
            snprintf(tmp_str, sizeof(tmp_str), "%d.%d℃", buf1[2], buf1[3]);
            // 格式化湿度字符串（整数部分.小数部分）
            snprintf(hum_str, sizeof(hum_str), "%d.%d%%", buf1[0], buf1[1]);
            shiduxianshi = atof(hum_str);  // 湿度值
            wenduxianshi = atof(tmp_str);  // 温度值
            // mode change
            if(!mode){
            if(buf1[2]>=temph)
            {
                if (!m_gpio->setPin(Fan)) {  // 使用GPIO17
                       qDebug() << "Failed to set GPIO pin"<<buf1[2];
                   }
                m_gpio->writeValue('1');  // 设置高电平
                if (!m_gpio->setPin(Hot)) {  // 使用GPIO17
                      qDebug() << "Failed to set GPIO pin";
                  }
                m_gpio->writeValue('0');  // 设置高电平
            }
            else if(buf1[2]<=templ)
            {
                if (!m_gpio->setPin(Hot)) {  // 使用GPIO17
                    qDebug() << "Failed to set GPIO pin";
                }
                m_gpio->writeValue('1');  // 设置高电平
                if (!m_gpio->setPin(Fan)) {  // 使用GPIO17
                  qDebug() << "Failed to set GPIO pin";
                }
                m_gpio->writeValue('0');  // 设置高电平
            }
            else
            {
                if (!m_gpio->setPin(Hot)) {  // 使用GPIO17
                    qDebug() << "Failed to set GPIO pin";
                }
                m_gpio->writeValue('0');  // 设置高电平
                if (!m_gpio->setPin(Fan)) {  // 使用GPIO17
                  qDebug() << "Failed to set GPIO pin";
                }
                m_gpio->writeValue('0');  // 设置高电平              
            }
            //湿度
            if(buf1[0]>=humh)
            {
                if (!m_gpio->setPin(Wu)) {  // 使用GPIO17
                  qDebug() << "Failed to set GPIO pin";
                }
                m_gpio->writeValue('0');  // 设置高电平
            }
            else if(buf1[0]<=huml)
            {
                if (!m_gpio->setPin(Wu)) {  // 使用GPIO17
                   qDebug() << "Failed to set GPIO pin";
                }
                m_gpio->writeValue('1');  // 设置高电平

            }
            else
            {
                if (!m_gpio->setPin(Wu)) {  // 使用GPIO17
                   qDebug() << "Failed to set GPIO pin";
                }
                m_gpio->writeValue('0');  // 设置高电平
            }
            }
            // 更新UI显示
            ui->label_tmp->setText(tmp_str);
            ui->label_hum->setText(hum_str);
        }
    }
    cnt++;
}

/*****************  control UVC Germicidal lamp ****************************/
static bool ledflag=false;
void MainWindow::on_pushButton_clicked(bool checked)
{
    updateLEDs();
    ledflag=!ledflag;

}
/***** UVC Germicidal lamp **/
void MainWindow::updateLEDs()
{
    QByteArray buf1[]={"0","1"};
    if(ledflag)
    {
        write(led_fd, buf1[0], 1);
        write(gpio_fd, buf1[0], 1);
        ui->pushButton->setStyleSheet("border-image:url(:/led1.png)");
    }
    else
    {
        write(led_fd, buf1[1], 1);
        write(gpio_fd, buf1[1], 1);
        ui->pushButton->setStyleSheet("border-image:url(:/led2.png)");
    }
}
/*****open UVC Germicidal lamp **/
void MainWindow::turnOnLED()
{
    QByteArray on = "1";
    write(led_fd, on.data(), 1);
    write(gpio_fd, on.data(), 1);
    ui->pushButton->setStyleSheet("border-image:url(:/led2.png)");
    ledflag = true;
}
/*****close UVC Germicidal lamp **/
void MainWindow::turnOffLED()
{
    QByteArray off = "0";
    write(led_fd, off.data(), 1);
    write(gpio_fd, off.data(), 1);
    ui->pushButton->setStyleSheet("border-image:url(:/led1.png)");
    ledflag = false;
}

/*****************  control food ****************************/
static bool isChecked = false;  // 手动维护状态
/*****auto food ***************/
void MainWindow::on_pushButton_4_clicked()
{
    updatafood();
}
/*****auto food **************/
void MainWindow::updatafood()
{
    openfood();
    QTimer::singleShot(foodweight*100, this, &MainWindow::closefood);
}
/*****open food **************/
void MainWindow::openfood()
{
    write(sg_fd, foodopen, sizeof(foodopen));
    ui->pushButton_4->setStyleSheet("border-image:url(:狗粮.png)");
}
/*****close food **************/
void MainWindow::closefood()
{
    write(sg_fd, foodclose, sizeof(foodclose));
    ui->pushButton_4->setStyleSheet("border-image:url(:喂食.png)");
}

/*****************  control door ****************************/
static bool doorflag=true;
void MainWindow::on_pushButton_5_clicked(bool checked)
{
   updatadoor();
   doorflag = !doorflag;
}
/*****updata door **************/
void MainWindow::updatadoor()
{
   if(doorflag){
    write(door_fd, doorclose, sizeof(doorclose));
    ui->pushButton_5->setStyleSheet("border-image:url(:关门.png)");
   }
   else{
    write(door_fd, dooropen, sizeof(dooropen));
    ui->pushButton_5->setStyleSheet("border-image:url(:开门.png)");
   }
}

/*****************  control led ****************************/
void MainWindow::on_toggleButton_clicked()
{
    isLedOn = !isLedOn; // 切换状态
    updateLedState();
    qDebug()<<"setok";
    printf("open RGB failed\n");
}
/**** updata led **********/
void MainWindow::updateLedState()
{
    QString program = "./test";
    QStringList arguments;
    QString led = (ledstate ? "5a2c0d":"070401");
    arguments << "/dev/spidev4.0" << (isLedOn ? led : "000000");
    if (process->state() == QProcess::Running) {
        process->kill();
    }
    process->start(program, arguments);
    QString style = isLedOn
        ? "border-image:url(:/灯带.png)"
        : "border-image:url(:/灯带1.png)";
    ui->toggleButton->setStyleSheet(style);
}

/*****************  control vocal assit ****************************************/
void MainWindow::onCommandReceived(int cmd, const QString &text)
{
    // 显示识别结果
    QString log = QString("识别到: %1 (0x%2)").arg(text).arg(cmd, 2, 16, QLatin1Char('0'));
    ui->textEdit->setText(log);
    // LED 控制部分
    static const QByteArray ledOn = "1";
    static const QByteArray ledOff = "0";
    if (led_fd != -1) {  // 检查文件描述符是否有效
        if (cmd == 0x12) {  // 正确的比较方式
            if (write(led_fd, ledOn.constData(), 1) == -1) {
                perror("LED on write failed");

            }
            isLedOn = true; // 切换状态
               updateLedState();
        }
        else if (cmd == 0x13) {
            if (write(led_fd, ledOff.constData(), 1) == -1) {
                perror("LED off write failed");

            }
            isLedOn = false; // 切换状态
               updateLedState();
        }
        else if (cmd == 0x14) {
            doorflag = true; // 切换状态
               updatadoor();
        }
        else if (cmd == 0x15) {
            doorflag = false; // 切换状态
               updatadoor();
        }
        else if (cmd == 0x01) {
            isChecked = true; // 切换状态
               updatafood();
        }
        else if (cmd == 0x03) {
            ledstate=1; // 切换状态
               updateLedState();
        }
        else if (cmd == 0x04) {
            ledstate=0; // 切换状态
               updateLedState();
        }
        else if (cmd == 0x24) {
            if (!m_gpio->setPin(Fan)) {  // 使用GPIO17
                   qDebug() << "Failed to set GPIO pin";
               }
               m_gpio->writeValue('1');  // 设置高电平
        }
        else if (cmd == 0x25) {
            if (!m_gpio->setPin(Fan)) {  // 使用GPIO17
                   qDebug() << "Failed to set GPIO pin";
               }
               m_gpio->writeValue('0');  // 设置高电平
        }
    } else {
        ui->textEdit->setText("LED设备未打开");
    }
    // 播报识别结果
    if (m_i2c) {
        m_i2c->speak(0x00, cmd); // 使用立即播报模式
    } else {
        ui->textEdit->setText("I2C设备未初始化");
    }
}

/*****************************TCP/IP***********************************/
void MainWindow::onDisconnected()
{
    qDebug() << "Disconnected!";
    // 停止并删除定时器
    if(sendTimer) {
        sendTimer->stop();
        delete sendTimer;
        sendTimer = nullptr;  // 重要：将指针置空
    }
    // 清理socket（重要）
    tcpSocket->deleteLater();
    tcpSocket = nullptr;

    QTimer::singleShot(5000, this, &MainWindow::startConnection);
}
void MainWindow::onConnected()
{
    qDebug() << "Connected successfully!";
    this->statusBar()->showMessage("Connected", 2000);
    // 确保定时器存在且只连接一次
    if(!sendTimer) {
        sendTimer = new QTimer(this);
        // 使用Qt5的新式连接语法，避免重复连接
        connect(sendTimer, &QTimer::timeout, this, &MainWindow::sendJsonData, Qt::UniqueConnection);
        sendTimer->start(1000);
    } else if(!sendTimer->isActive()) {
        sendTimer->start(1000);
    }
}
void MainWindow::startConnection()
{
    // 确保socket被清理
    if(tcpSocket) {
        tcpSocket->deleteLater();
        tcpSocket = nullptr;
    }
    setupConnections();  // 重新初始化连接
    QString ip = "172.20.10.4";
    quint16 port = 12345;
    qDebug() << "Reconnecting to" << ip << ":" << port;
    tcpSocket->connectToHost(ip, port);
}
void MainWindow::setupConnections()
{
    // 创建新的socket实例
    tcpSocket = new QTcpSocket(this);

    // 使用Qt::UniqueConnection防止重复连接
    connect(tcpSocket, &QTcpSocket::connected,
            this, &MainWindow::onConnected, Qt::UniqueConnection);
    connect(tcpSocket, &QTcpSocket::disconnected,
            this, &MainWindow::onDisconnected, Qt::UniqueConnection);
    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this, &MainWindow::onSocketError, Qt::UniqueConnection);
    connect(tcpSocket, &QTcpSocket::readyRead,
            this, &MainWindow::processReceivedData, Qt::UniqueConnection);
}
void MainWindow::onSocketError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error);
    qDebug() << "Connection error:" << tcpSocket->errorString();
    this->statusBar()->showMessage("Error: " + tcpSocket->errorString(), 2000);
    QTimer::singleShot(3000, this, &MainWindow::startConnection);
}
// 发送JSON数据
void MainWindow::sendJsonData()
{
    if(tcpSocket && tcpSocket->state() == QAbstractSocket::ConnectedState) {
        QJsonObject json = createJsonObject();
        QByteArray jsonData = QJsonDocument(json).toJson();

        if(tcpSocket->write(jsonData) == -1) {
            qDebug() << "Failed to send data:" << tcpSocket->errorString();
        } else {
            qDebug() << "Sent JSON:" << jsonData;
            ui->sendEdit->setText("Last sent: " + QDateTime::currentDateTime().toString());
        }
    }
}

// 创建JSON对象
QJsonObject MainWindow::createJsonObject()
{
    QJsonObject json;
    json["temperature"] = QString::number(wenduxianshi, 'f', 2).toDouble();
    json["humidity"] = QString::number(shiduxianshi, 'f', 2).toDouble();
    json["WC"] = WCNUm;
    json["water"] = watersend;
    json["weight"] = weightsend;
    json["sun"] = sun;
    json["foodste"] = isChecked;
    json["ledstate"] = ledstate;//调亮暗
    json["led"] =isLedOn ;//灯光打开关闭
    json["doorstate"] = doorflag;
    json["tiwen"] = QString::number(tiwen, 'f', 2).toDouble();

    return json;
}

void MainWindow::processReceivedData()
{
    while(tcpSocket->bytesAvailable() > 0) {
        QByteArray data = tcpSocket->readAll();
        QString receivedStr = QString::fromUtf8(data).trimmed();

        // 显示原始数据（调试用）
        ui->recvEdit->appendPlainText("[RAW] " + receivedStr);

        // 检查JSON格式有效性
        if(!receivedStr.startsWith("{") || !receivedStr.endsWith("}")) {
            qDebug() << "非JSON格式数据";
            return;
        }

        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(receivedStr.toUtf8(), &error);

        if(error.error != QJsonParseError::NoError) {
            qDebug() << "JSON解析错误:" << error.errorString();
            ui->recvEdit->appendPlainText("错误: " + error.errorString());
            return;
        }

        QJsonObject json = doc.object();
        QStringList logMessages;

        // 1. 解析温度阈值
        if(json.contains("temph") || json.contains("templ")) {
            temph = json.value("temph").toInt(); // 保持原值如果字段不存在
            templ = json.value("templ").toInt();
            logMessages << QString("温度阈值: %1~%2°C").arg(templ).arg(temph);
        }

        // 2. 解析湿度阈值
        if(json.contains("humh") || json.contains("huml")) {
            humh = json.value("humh").toInt(humh);
            huml = json.value("huml").toInt(huml);
            logMessages << QString("湿度阈值: %1~%2%").arg(huml).arg(humh);
        }

        // 3. 解析设备状态
        if(json.contains("foodweight")) {
            foodweight = json["foodweight"].toInt();
            logMessages << "饲料重量: " + QString::number(foodweight) + "kg";
        }

        if(json.contains("xiaodutime")) {
            xiaodutime = json["xiaodutime"].toInt();
            logMessages << "消毒时间: " + QString::number(xiaodutime) + "秒";
        }

        if(json.contains("lightstate")) {
            ledstate = json["lightstate"].toBool();
             updateLedState(); // 更新LED状态
            logMessages << "灯光状态: " + QString(ledstate ? "ON" : "OFF");

        }

        if(json.contains("ledstate")) {
            isLedOn = json["ledstate"].toBool();
             updateLedState(); // 更新LED状态
            logMessages << "LED控制: " + QString(isLedOn ? "ON" : "OFF");
        }

        if(json.contains("doorstate")) {
            doorflag = json["doorstate"].toBool();

            updatadoor(); // 更新门状态
            logMessages << "门状态: " + QString(doorflag ? "开启" : "关闭");
        }

        if(json.contains("foodstate")) {
            isChecked = json["foodstate"].toBool();
            if(isChecked)updatafood(); // 更新门状态
            logMessages << "喂食状态: " + QString(isChecked ? "开启" : "关闭");
        }

        // 5. 处理特殊指令
        if(json.contains("next")) {
            int nextAction = json["next"].toBool();
            logMessages << "下一步指令: " + QString::number(nextAction);
            // 添加具体指令处理逻辑...
        }
        // 显示解析结果
        ui->recvEdit->appendPlainText("---- 解析结果 ----");
        for(const auto &msg : logMessages) {
            qDebug()<<msg<<"\n";
            ui->recvEdit->appendPlainText(msg);
        }
        ui->recvEdit->appendPlainText("----------------\n");
    }
}
// TCB BUTTON
void MainWindow::on_openBt_clicked()
{
    if(tcpSocket && tcpSocket->state() == QAbstractSocket::ConnectedState) {
        tcpSocket->disconnectFromHost();
    }
    startConnection();
}
void MainWindow::on_closeBt_clicked()
{
    if(sendTimer) {
        sendTimer->stop();
        delete sendTimer;
        sendTimer = nullptr;
    }
    if(tcpSocket) {
        tcpSocket->disconnectFromHost();
    }
}
/*************************************time************************/
void MainWindow::slot_timeout(void)
{
    ui->label_time_1->setText(QTime::currentTime().toString("hh:mm"));
    ui->label_time_2->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd ddd"));
    qDebug() << "时间:" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
}
/***********************************change page******************/
void MainWindow::on_pushButton_6_clicked(bool checked)
{
    heart *ct = new heart;
    mode = true;
    ct->setGeometry(this->geometry());
    ct->show();
}


/***********************************change body temp******************/
void MainWindow::onTemperatureUpdated(float objectTemp, float ambientTemp)
{
    ui->tempEdit->setText(QString("%1 °C").arg(objectTemp, 0, 'f', 1));
}
// 传感器错误槽函数
void MainWindow::onSensorError(const QString &message)
{
    ui->tempEdit->setText("传感器错误");
    statusBar()->showMessage(message, 5000); // 显示5秒错误信息
    qWarning() << "MLX90614 Error:" << message;
}
void MainWindow::updateTemperature()
{
    float temp;
    if (m_sensor->readTemperature(temp)) {
        ui->tempEdit->setText(QString::number(temp, 'f', 2) + "°C  ");
        tiwen = temp;
    } else {
        ui->tempEdit->setText("读取失败");
    }
}
void MainWindow::handleError(const QString &msg)
{
    qWarning() << "Error:" << msg;
}
/********************************************************music********************************/
/*****************析构函数**************/
MainWindow::~MainWindow()
{
    m_terminating = true;
    stopAllPlayers();
    delete ui;
}
void MainWindow::stopAllPlayers()
{
    qDebug() << "Stopping all players...";
    cleanupPlayer(m_comfortProcess, ui->playComfortButton);
    cleanupPlayer(m_sleepProcess, ui->playSleepButton);
}

void MainWindow::cleanupPlayer(QProcess* &process, QPushButton *button)
{
    if (process) {
        qDebug() << "Cleaning up process for button:" << button->objectName();

        // 断开所有信号连接
        disconnect(process, nullptr, this, nullptr);

        // 终止进程
        process->terminate();
        if (!process->waitForFinished(300)) {
            qDebug() << "Force killing process...";
            process->kill();
            process->waitForFinished(100);
        }

        // 更新按钮状态（确保在主线程执行）
        QMetaObject::invokeMethod(this, [this, button](){
            updateButtonState(button, false);
        }, Qt::QueuedConnection);

        delete process;
        process = nullptr;
        qDebug() << "Cleanup completed";
    }
}

void MainWindow::setupGStreamerEnvironment(QProcess *process)
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("LD_LIBRARY_PATH", "/usr/lib/aarch64-linux-gnu/gstreamer-1.0");
    env.insert("GST_PLUGIN_PATH", "/usr/lib/aarch64-linux-gnu/gstreamer-1.0");
    process->setProcessEnvironment(env);
}

bool MainWindow::startPlayer(const QString &filePath, QProcess* &process, QPushButton *button)
{
    if (!checkAudioFile(filePath)) {
        return false;
    }

    // 停止当前播放（如果有）
    if (process && process->state() == QProcess::Running) {
        cleanupPlayer(process, button);
    }

    process = new QProcess(this);
    process->setProcessChannelMode(QProcess::SeparateChannels);
    setupGStreamerEnvironment(process);

    // 连接进程信号
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        [this, button](int exitCode, QProcess::ExitStatus status) {
            qDebug() << "Player finished with code:" << exitCode;
            updateButtonState(button, false);
        });

    connect(process, &QProcess::errorOccurred, [this, button](QProcess::ProcessError error) {
        qWarning() << "Playback error:" << error;
        updateButtonState(button, false);
    });

    // 启动播放
    process->start("gst-play-1.0", {
        "--no-interactive",
        "--audiosink=pulsesink",
        filePath
    });

    if (process->waitForStarted(1000)) {
        updateButtonState(button, true);
        return true;
    } else {
        qWarning() << "Failed to start player";
        updateButtonState(button, false);
        return false;
    }
}

void MainWindow::updateButtonState(QPushButton *button, bool isPlaying)
{
    if (!button) return;

    qDebug() << "Updating button" << button->objectName() << "to state:" << isPlaying;
    button->setChecked(isPlaying);

    // 使用样式表确保图标切换
    if (button == ui->playComfortButton) {
        button->setStyleSheet(isPlaying
            ? "QPushButton { border-image: url(:音乐(3).png); }"
            : "QPushButton { border-image: url(:音乐(2).png); }");
    }
    else if (button == ui->playSleepButton) {
        button->setStyleSheet(isPlaying
            ? "QPushButton { border-image: url(:音乐(1).png); }"
            : "QPushButton { border-image: url(:音乐.png); }");
    }

    // 强制重绘
    button->repaint();
}

bool MainWindow::checkAudioFile(const QString &path)
{
    if (!QFile::exists(path)) {
        qWarning() << "Audio file not found:" << path;
        return false;
    }
    return true;
}

void MainWindow::on_playComfortButton_clicked(bool checked)
{
    qDebug() << "Comfort button clicked, checked:" << checked;

    if (checked) {
        if (startPlayer(m_comfortAudioPath, m_comfortProcess, ui->playComfortButton)) {
            qDebug() << "Comfort music started";
        }
    } else {
        if (m_comfortProcess) {
            cleanupPlayer(m_comfortProcess, ui->playComfortButton);
        }
    }
}

void MainWindow::on_playSleepButton_clicked(bool checked)
{
    qDebug() << "Sleep button clicked, checked:" << checked;

    if (checked) {
        if (startPlayer(m_sleepAudioPath, m_sleepProcess, ui->playSleepButton)) {
            qDebug() << "Sleep music started";
        }
    } else {
        if (m_sleepProcess) {
            cleanupPlayer(m_sleepProcess, ui->playSleepButton);
        }
    }
}
