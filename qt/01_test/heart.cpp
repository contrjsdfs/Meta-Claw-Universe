#include "heart.h"
#include "ui_heart.h"
#include "mainwindow.h"
#include "gpiodevice.h"
#include <QMessageBox>
#include <QDateTime>  // 如果你使用了 QDateTime
#include "ui_mainwindow.h"  // 必须包含

#define Fan     104
#define Wu      123
#define Hot     114
#define Cold    115

#define foodopen "2100000"
#define foodclose "600000"

#define sg90_export                 "/sys/class/pwm/pwmchip0/export"
#define sg90_period                 "/sys/class/pwm/pwmchip0/pwm0/period"
#define sg90_duty                   "/sys/class/pwm/pwmchip0/pwm0/duty_cycle"
#define sg90_polarity               "/sys/class/pwm/pwmchip0/pwm0/polarity"
#define sg90_able                   "/sys/class/pwm/pwmchip0/pwm0/enable"

heart::heart(QWidget *parent) :
    QWidget(parent),
    m_gpio(new GpioDevice(this)),
    ui(new Ui::heart)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    // 设置背景透明 + 字体白色
    this->setStyleSheet(
        // 基本 QTimeEdit 样式
        "QTimeEdit {"
        "    background: rgba(0, 0, 0, 150);"  // 半透明黑色背景
        "    color: white;"                   // 字体白色
        "    border: 1px solid gray;"         // 边框
        "    padding-right: 35px;"           // 为放大按钮预留更多空间
        "    width: 100px;"              // 设置最小宽度
        "    height: 60px;"              // 设置最小高度
        "}"

        // 放大上按钮
        "QTimeEdit::up-button {"
        "    width: 45px;"                   // 按钮宽度
        "    height: 30px;"                  // 按钮高度
        "    subcontrol-origin: border;"
        "    subcontrol-position: top right;" // 位置在右上
//        "    border-left: 1px solid gray;"   // 添加分割线
//        "    //image: url(:/images/up_arrow.png);" // 可选：自定义箭头图标
        "}"

        // 放大下按钮
        "QTimeEdit::down-button {"
        "    width: 45px;"                   // 按钮宽度
        "    height: 30px;"                  // 按钮高度
        "    subcontrol-origin: border;"
        "    subcontrol-position: bottom right;" // 位置在右下
//    //    "    border-left: 1px solid gray;"    // 添加分割线
//        "   // image: url(:/images/down_arrow.png);" // 可选：自定义箭头图标
        "}"

//        // 按钮悬停效果
//        "QTimeEdit::up-button:hover, QTimeEdit::down-button:hover {"
//        "    background: rgba(255, 255, 255, 30);" // 悬停时轻微高亮
//        "}"

//        // 按钮按下效果
//        "QTimeEdit::up-button:pressed, QTimeEdit::down-button:pressed {"
//        "    background: rgba(255, 255, 255, 50);" // 按下时更强的高亮
//        "}"
    );
       // 确保背景透明
       this->setAttribute(Qt::WA_TranslucentBackground);

    ui->stopButton->setEnabled(false);

        // 设置默认喂食时间(8:00, 12:00, 18:00)
        ui->timeEdit1->setTime(QTime(8, 30));
        ui->timeEdit2->setTime(QTime(12, 0));
        ui->timeEdit3->setTime(QTime(18, 0));
        //ui->timeEdit4->setTime(QTime(18, 0));
        // 初始化定时器
        checkTimer = new QTimer(this);
        connect(checkTimer, &QTimer::timeout, this, &heart::checkFeedingTime);

        // 更新喂食时间列表
        updateFeedingTimes();


}

heart::~heart()
{

    delete ui;
}

void heart::on_pushButton_clicked()
{
    MainWindow *ct = new MainWindow;
    mode = false;
    ct->setGeometry(this->geometry());
    ct->show();
}



void heart::on_pushButton_3_clicked(bool checked)
{
    if(mode == true && checked)
    {
        if (!m_gpio->setPin(Wu)) {  // 使用GPIO17
               qDebug() << "Failed to set GPIO pin";

           }
        ui->pushButton_3->setStyleSheet("border-image:url(:全雾化.png)");
           // 写入GPIO值
           m_gpio->writeValue('1');  // 设置高电平
    }
    else if(mode == true && checked==0)
    {
        if (!m_gpio->setPin(Wu)) {  // 使用GPIO17
                qDebug() << "Failed to set GPIO pin";
 
            }

        ui->pushButton_3->setStyleSheet("border-image:url(:关雾化.png)");
        m_gpio->writeValue('0');  // 设置高电平
    }

}

void heart::on_pushButton_7_clicked(bool checked)
{
    if(mode == true && checked)
    {
        if(!m_gpio->setPin(Fan)) {  // 使用GPIO17
                       qDebug() << "Failed to set GPIO pin";
                   }

                   // 写入GPIO值
                   m_gpio->writeValue('1');  // 设置高电平
                   ui->pushButton_7->setStyleSheet("border-image:url(:风扇.png)");
    }
    else if(mode == true && checked==0)
    {
        if (!m_gpio->setPin(Fan)) {  // 使用GPIO17
               qDebug() << "Failed to set GPIO pin";
           }

           // 写入GPIO值
           m_gpio->writeValue('0');  // 设置高电平
           ui->pushButton_7->setStyleSheet("border-image:url(:风扇(2).png)");
    }
}

void heart::on_pushButton_8_clicked(bool checked)
{
    if(mode == true && checked)
    {
        if (!m_gpio->setPin(Hot)) {  // 使用GPIO17
               qDebug() << "Failed to set GPIO pin";

           }
        ui->pushButton_8->setStyleSheet("border-image:url(:加热(1).png)");
           // 写入GPIO值
           m_gpio->writeValue('1');  // 设置高电平
    }
    else if(mode == true && checked==0)
    {
        if (!m_gpio->setPin(Hot)) {  // 使用GPIO17
               qDebug() << "Failed to set GPIO pin";
           }

           // 写入GPIO值
           m_gpio->writeValue('0');  // 设置高电平
           ui->pushButton_8->setStyleSheet("border-image:url(:加热.png)");
    }
}



void heart::updateFeedingTimes()
{
    feedingTimes.clear();
    feedingTimes.append(ui->timeEdit1->time());
    feedingTimes.append(ui->timeEdit2->time());
    feedingTimes.append(ui->timeEdit3->time());

    // 按时间排序确保顺序
    std::sort(feedingTimes.begin(), feedingTimes.end());

//    // 更新时间显示
//    QString timesStr = "当前喂食时间: ";
//    for (const QTime &time : feedingTimes) {
//        timesStr += time.toString("hh:mm") + "  ";
//    }
//    ui->statusLabel->setText(timesStr);

   // qDebug() << "喂食时间已更新:" << timesStr;
}

// 修改checkFeedingTime函数
void heart::checkFeedingTime()
{
    QDateTime current = QDateTime::currentDateTime();
    QTime currentTime = current.time();

    qDebug() << "检查喂食时间 - 当前时间:" << current.toString("yyyy-MM-dd hh:mm:ss");

    for (const QTime &feedingTime : feedingTimes) {
        // 计算时间差（秒）
        int secsDiff = qAbs(currentTime.secsTo(feedingTime));

        qDebug() << "检查时间点:" << feedingTime.toString("hh:mm")
                 << "时间差:" << secsDiff << "秒";

        // 在准确时间点±5秒内触发
        if (secsDiff <= 5) {
            // 防止重复触发 - 检查上次触发时间
            if (!lastFeedingTimes.contains(feedingTime) ||
                lastFeedingTimes[feedingTime].secsTo(current) >= 3600) {

                qDebug() << "触发喂食:" << feedingTime.toString("hh:mm");
                activateServo();

                // 记录日志
//                QString msg = QString("在 %1 执行喂食操作").arg(feedingTime.toString("hh:mm"));
//                ui->logTextEdit->append(QString("[%1] %2")
//                    .arg(current.toString("yyyy-MM-dd hh:mm:ss"), msg));
            }
        }
    }
}
//void heart::performFeeding(const QString &timeStr)
//{
//    QString msg = QString("在 %1 执行喂食操作").arg(timeStr);
//    QMessageBox::information(this, "喂食", msg);
//    ui->logTextEdit->append(QString("[%1] %2").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"), msg));
//}

void heart::on_startButton_clicked()
{
    updateFeedingTimes();
    checkTimer->start(1000); // 每秒检查一次时间
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    ui->timeEdit1->setEnabled(false);
    ui->timeEdit2->setEnabled(false);
    ui->timeEdit3->setEnabled(false);
    ui->tempEdit->setText("已开启定时喂食");
}

void heart::on_stopButton_clicked()
{
    checkTimer->stop();
    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    ui->timeEdit1->setEnabled(true);
    ui->timeEdit2->setEnabled(true);
    ui->timeEdit3->setEnabled(true);
    ui->tempEdit->setText("已关闭定时喂食");
}

void heart::activateServo()
{
    /*duoji*/
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

     openfood();
//     updateLedopen(); // 打开LED

        // 设置3秒后关闭食物通道
        QTimer::singleShot(foodweight*300, this, [this](){
            closefood();
        });

//        // 设置1小时后关闭LED
//        ledOffTimer->start(3600000);
//            qDebug() << "已启动1小时定时器，将在1小时后关闭LED";
 }
 void heart::openfood()
 {
     write(sg_fd, foodopen, sizeof(foodopen));
    // ui->pushButton_4->setStyleSheet("border-image:url(:狗粮.png)");
 }
 void heart::closefood()
 {
     write(sg_fd, foodclose, sizeof(foodclose));
     ::close(sg_fd);  // 正确关闭文件描述符（使用 ::close 系统调用）
         sg_fd = -1;      // 标记为无效，避免重复关闭
     //ui->pushButton_4->setStyleSheet("border-image:url(:喂食.png)");
 }
// void heart::updateLedopen()
// {
//     QString program = "./test";
//     QStringList arguments;
//     QString led = (ledstate ? "5a2c0d":"070401");
//     arguments << "/dev/spidev4.0" << (led);

//     if (process->state() == QProcess::Running) {
//         process->kill();
//     }

//     process->start(program, arguments);
////     QString style = isLedOn
////         ? "border-image:url(:/灯带.png)"
////         : "border-image:url(:/灯带1.png)";

////     ui->toggleButton->setStyleSheet(style);
//     // 更新按钮文本
//    // ui->toggleButton->setStyleSheet();
// }
// void heart::updateLedclose()
// {
//     QString program = "./test";
//     QStringList arguments;
//     QString led = (ledstate ? "5a2c0d":"070401");
//     arguments << "/dev/spidev4.0" << "000000";

//     if (process->state() == QProcess::Running) {
//         process->kill();
//     }

//     process->start(program, arguments);
////     QString style = isLedOn
////         ? "border-image:url(:/灯带.png)"
////         : "border-image:url(:/灯带1.png)";

////     ui->toggleButton->setStyleSheet(style);
//     // 更新按钮文本
//    // ui->toggleButton->setStyleSheet();
// }
