#ifndef HEART_H
#define HEART_H

#include <QWidget>
#include "gpiodevice.h"
#include <QTimer>
#include <QTime>
#include <QVector>
#include <QMap>      // 添加这行
#include <QTime>     // 添加这行
#include <QDateTime> // 添加这行
#include <QProcess>
namespace Ui {
class heart;
}

class heart : public QWidget
{
    Q_OBJECT

public:
    explicit heart(QWidget *parent = nullptr);
    ~heart();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked(bool checked);

    void on_pushButton_7_clicked(bool checked);

    void on_pushButton_8_clicked(bool checked);

    void checkFeedingTime();
        void on_startButton_clicked();
        void on_stopButton_clicked();

        void updateFeedingTimes();

       void activateServo();
       void closefood();
       void openfood();
//       void updateLedopen();
//       void updateLedclose();
       //void activateServo();
private:
    Ui::heart *ui;
    GpioDevice *m_gpio;  // 添加这行声明
    QTimer *checkTimer;
   QVector<QTime> feedingTimes;
   QMap<QTime, QDateTime> lastFeedingTimes; // 记录上次喂食时间
   QProcess *process;
  // QTimer *ledOffTimer;  // 用于控制LED关闭的定时器
   int sg_fd;
   //void performFeeding(const QString &timeStr);
};

#endif // HEART_H
