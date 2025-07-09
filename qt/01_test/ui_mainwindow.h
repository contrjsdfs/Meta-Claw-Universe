/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_ir;
    QLabel *label_light;
    QLabel *label_dis;
    QLabel *label_tmp;
    QLabel *label_hum;
    QPushButton *pushButton_4;
    QLabel *label_7;
    QLabel *label_people;
    QLabel *label_8;
    QLabel *label_air;
    QPushButton *pushButton_5;
    QPushButton *toggleButton;
    QPlainTextEdit *recvEdit;
    QLineEdit *sendEdit;
    QLineEdit *portEdit;
    QLineEdit *IPEdit;
    QPushButton *openBt;
    QPushButton *closeBt;
    QPushButton *sendBt;
    QLabel *label_9;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_time_1;
    QLabel *label_time_2;
    QListView *listView;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *centralLayout;
    QLineEdit *textEdit;
    QLabel *label_10;
    QLabel *label_15;
    QPushButton *pushButton_heart;
    QPushButton *pushButton_6;
    QLabel *tempEdit;
    QLabel *label_14;
    QPushButton *playComfortButton;
    QPushButton *playSleepButton;
    QLabel *label_16;
    QLabel *label_17;
    QLabel *tempEdit_2;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->setEnabled(true);
        MainWindow->resize(1024, 600);
        MainWindow->setMinimumSize(QSize(1024, 600));
        MainWindow->setMaximumSize(QSize(1024, 600));
        MainWindow->setStyleSheet(QString::fromUtf8("#MainWindow{background-image: url(:/1.png);}"));
        MainWindow->setAnimated(true);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(610, 480, 101, 91));
        QFont font;
        font.setPointSize(20);
        pushButton->setFont(font);
        pushButton->setStyleSheet(QString::fromUtf8("border-image: url(:/led1.png);"));
        pushButton->setCheckable(true);
        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(890, 480, 101, 101));
        pushButton_2->setFont(font);
        pushButton_2->setLayoutDirection(Qt::LeftToRight);
        pushButton_2->setStyleSheet(QString::fromUtf8("border-image: url(:/\344\270\215\345\212\240\346\260\264.png);"));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 450, 141, 34));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Ubuntu Condensed"));
        font1.setPointSize(20);
        font1.setBold(false);
        font1.setItalic(true);
        font1.setWeight(9);
        label_3->setFont(font1);
        label_3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 75 italic 20pt \"Ubuntu Condensed\";"));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 490, 81, 34));
        label_4->setFont(font1);
        label_4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 75 italic 20pt \"Ubuntu Condensed\";"));
        label_ir = new QLabel(centralwidget);
        label_ir->setObjectName(QString::fromUtf8("label_ir"));
        label_ir->setGeometry(QRect(250, 530, 71, 34));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Ubuntu Condensed"));
        font2.setPointSize(20);
        font2.setBold(false);
        font2.setItalic(true);
        font2.setWeight(50);
        label_ir->setFont(font2);
        label_ir->setStyleSheet(QString::fromUtf8("color: transparent;\n"
"background:transparent;border-width:0;border-style:outset"));
        label_light = new QLabel(centralwidget);
        label_light->setObjectName(QString::fromUtf8("label_light"));
        label_light->setGeometry(QRect(110, 490, 161, 34));
        label_light->setFont(font1);
        label_light->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 75 italic 20pt \"Ubuntu Condensed\";"));
        label_dis = new QLabel(centralwidget);
        label_dis->setObjectName(QString::fromUtf8("label_dis"));
        label_dis->setGeometry(QRect(130, 530, 161, 34));
        label_dis->setFont(font1);
        label_dis->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 75 italic 20pt \"Ubuntu Condensed\";"));
        label_tmp = new QLabel(centralwidget);
        label_tmp->setObjectName(QString::fromUtf8("label_tmp"));
        label_tmp->setGeometry(QRect(20, 300, 171, 51));
        label_tmp->setFont(font2);
        label_tmp->setStyleSheet(QString::fromUtf8("color: transparent;\n"
"background:transparent;border-width:0;border-style:outset"));
        label_hum = new QLabel(centralwidget);
        label_hum->setObjectName(QString::fromUtf8("label_hum"));
        label_hum->setGeometry(QRect(300, 350, 41, 34));
        label_hum->setFont(font2);
        label_hum->setStyleSheet(QString::fromUtf8("color: transparent;\n"
"background:transparent;border-width:0;border-style:outset\n"
""));
        pushButton_4 = new QPushButton(centralwidget);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(750, 490, 101, 91));
        pushButton_4->setFont(font);
        pushButton_4->setStyleSheet(QString::fromUtf8("border-image: url(:/\345\226\202\351\243\237.png);"));
        pushButton_4->setCheckable(true);
        pushButton_4->setChecked(true);
        label_7 = new QLabel(centralwidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(10, 530, 121, 34));
        label_7->setFont(font1);
        label_7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 75 italic 20pt \"Ubuntu Condensed\";"));
        label_people = new QLabel(centralwidget);
        label_people->setObjectName(QString::fromUtf8("label_people"));
        label_people->setGeometry(QRect(140, 450, 81, 34));
        label_people->setFont(font1);
        label_people->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 75 italic 20pt \"Ubuntu Condensed\";"));
        label_8 = new QLabel(centralwidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(10, 410, 121, 34));
        label_8->setFont(font1);
        label_8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 75 italic 20pt \"Ubuntu Condensed\";"));
        label_air = new QLabel(centralwidget);
        label_air->setObjectName(QString::fromUtf8("label_air"));
        label_air->setGeometry(QRect(130, 410, 111, 34));
        label_air->setFont(font1);
        label_air->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 75 italic 20pt \"Ubuntu Condensed\";"));
        pushButton_5 = new QPushButton(centralwidget);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setGeometry(QRect(350, 490, 91, 91));
        QFont font3;
        font3.setPointSize(19);
        pushButton_5->setFont(font3);
        pushButton_5->setStyleSheet(QString::fromUtf8("border-image: url(:/\345\205\263\351\227\250.png);"));
        pushButton_5->setCheckable(true);
        toggleButton = new QPushButton(centralwidget);
        toggleButton->setObjectName(QString::fromUtf8("toggleButton"));
        toggleButton->setGeometry(QRect(470, 480, 101, 111));
        toggleButton->setStyleSheet(QString::fromUtf8("border-image: url(:/\347\201\257\345\270\2461.png);"));
        recvEdit = new QPlainTextEdit(centralwidget);
        recvEdit->setObjectName(QString::fromUtf8("recvEdit"));
        recvEdit->setGeometry(QRect(10, 30, 291, 161));
        recvEdit->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background:transparent;border-width:0;border-style:outset"));
        sendEdit = new QLineEdit(centralwidget);
        sendEdit->setObjectName(QString::fromUtf8("sendEdit"));
        sendEdit->setGeometry(QRect(10, 220, 291, 61));
        sendEdit->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background:transparent;border-width:0;border-style:outset"));
        portEdit = new QLineEdit(centralwidget);
        portEdit->setObjectName(QString::fromUtf8("portEdit"));
        portEdit->setGeometry(QRect(310, 30, 151, 41));
        portEdit->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background:transparent;border-width:0;border-style:outset\n"
""));
        IPEdit = new QLineEdit(centralwidget);
        IPEdit->setObjectName(QString::fromUtf8("IPEdit"));
        IPEdit->setGeometry(QRect(310, 90, 151, 41));
        IPEdit->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background:transparent;border-width:0;border-style:outset"));
        openBt = new QPushButton(centralwidget);
        openBt->setObjectName(QString::fromUtf8("openBt"));
        openBt->setGeometry(QRect(310, 140, 89, 25));
        openBt->setStyleSheet(QString::fromUtf8("color: transparent;\n"
"background:transparent;border-width:0;border-style:outset"));
        closeBt = new QPushButton(centralwidget);
        closeBt->setObjectName(QString::fromUtf8("closeBt"));
        closeBt->setGeometry(QRect(310, 180, 89, 25));
        closeBt->setStyleSheet(QString::fromUtf8("color: transparent;\n"
"background:transparent;border-width:0;border-style:outset"));
        sendBt = new QPushButton(centralwidget);
        sendBt->setObjectName(QString::fromUtf8("sendBt"));
        sendBt->setGeometry(QRect(310, 220, 89, 25));
        sendBt->setStyleSheet(QString::fromUtf8("color: transparent;\n"
"background:transparent;border-width:0;border-style:outset"));
        label_9 = new QLabel(centralwidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(20, 10, 67, 17));
        label_9->setStyleSheet(QString::fromUtf8("color: transparent;\n"
"background:transparent;border-width:0;border-style:outset"));
        label_11 = new QLabel(centralwidget);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(20, 200, 67, 17));
        label_11->setStyleSheet(QString::fromUtf8("color: transparent;\n"
"background:transparent;border-width:0;border-style:outset"));
        label_12 = new QLabel(centralwidget);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(310, 10, 67, 17));
        label_12->setStyleSheet(QString::fromUtf8("color: transparent;\n"
"background:transparent;border-width:0;border-style:outset"));
        label_13 = new QLabel(centralwidget);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(310, 70, 67, 20));
        label_13->setStyleSheet(QString::fromUtf8("color: transparent;\n"
"background:transparent;border-width:0;border-style:outset"));
        label_time_1 = new QLabel(centralwidget);
        label_time_1->setObjectName(QString::fromUtf8("label_time_1"));
        label_time_1->setGeometry(QRect(460, 10, 311, 51));
        QFont font4;
        font4.setPointSize(45);
        font4.setBold(true);
        font4.setItalic(true);
        font4.setWeight(75);
        label_time_1->setFont(font4);
        label_time_1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_time_2 = new QLabel(centralwidget);
        label_time_2->setObjectName(QString::fromUtf8("label_time_2"));
        label_time_2->setGeometry(QRect(460, 70, 491, 71));
        QFont font5;
        font5.setPointSize(30);
        font5.setBold(true);
        font5.setItalic(true);
        font5.setWeight(75);
        label_time_2->setFont(font5);
        label_time_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        listView = new QListView(centralwidget);
        listView->setObjectName(QString::fromUtf8("listView"));
        listView->setEnabled(false);
        listView->setGeometry(QRect(0, -10, 1061, 621));
        QFont font6;
        font6.setKerning(false);
        listView->setFont(font6);
#if QT_CONFIG(whatsthis)
        listView->setWhatsThis(QString::fromUtf8(""));
#endif // QT_CONFIG(whatsthis)
        listView->setStyleSheet(QString::fromUtf8("border-image: url(:/\350\203\214\346\231\257\345\233\2761.png);"));
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(830, 140, 91, 271));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayoutWidget_2 = new QWidget(centralwidget);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(900, 130, 91, 291));
        centralLayout = new QVBoxLayout(verticalLayoutWidget_2);
        centralLayout->setObjectName(QString::fromUtf8("centralLayout"));
        centralLayout->setContentsMargins(0, 0, 0, 0);
        textEdit = new QLineEdit(centralwidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(270, 350, 121, 41));
        textEdit->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background:transparent;border-width:0;border-style:outset"));
        label_10 = new QLabel(centralwidget);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(920, 410, 61, 51));
        label_10->setStyleSheet(QString::fromUtf8("border-image: url(:/\346\270\251\345\272\246\350\256\241.png);"));
        label_15 = new QLabel(centralwidget);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(840, 420, 51, 41));
        label_15->setStyleSheet(QString::fromUtf8("border-image: url(:/\346\260\264\346\273\264.png);"));
        pushButton_heart = new QPushButton(centralwidget);
        pushButton_heart->setObjectName(QString::fromUtf8("pushButton_heart"));
        pushButton_heart->setGeometry(QRect(470, 220, 71, 71));
        pushButton_heart->setStyleSheet(QString::fromUtf8("border-image: url(:/\345\201\245\345\272\267.png);"));
        pushButton_6 = new QPushButton(centralwidget);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        pushButton_6->setGeometry(QRect(240, 500, 89, 71));
        pushButton_6->setStyleSheet(QString::fromUtf8("border-image: url(:/\346\211\213\345\212\250.png);\n"
"border-image: url(:/auto.png);"));
        pushButton_6->setCheckable(true);
        tempEdit = new QLabel(centralwidget);
        tempEdit->setObjectName(QString::fromUtf8("tempEdit"));
        tempEdit->setGeometry(QRect(110, 370, 301, 34));
        tempEdit->setFont(font1);
        tempEdit->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 75 italic 20pt \"Ubuntu Condensed\";"));
        label_14 = new QLabel(centralwidget);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(20, 370, 81, 34));
        label_14->setFont(font1);
        label_14->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 75 italic 20pt \"Ubuntu Condensed\";"));
        playComfortButton = new QPushButton(centralwidget);
        playComfortButton->setObjectName(QString::fromUtf8("playComfortButton"));
        playComfortButton->setGeometry(QRect(30, 80, 89, 91));
        playComfortButton->setStyleSheet(QString::fromUtf8("border-image: url(:/\351\237\263\344\271\220(2).png);"));
        playComfortButton->setCheckable(true);
        playSleepButton = new QPushButton(centralwidget);
        playSleepButton->setObjectName(QString::fromUtf8("playSleepButton"));
        playSleepButton->setGeometry(QRect(180, 80, 81, 91));
        playSleepButton->setStyleSheet(QString::fromUtf8("border-image: url(:/\351\237\263\344\271\220.png);"));
        playSleepButton->setCheckable(true);
        label_16 = new QLabel(centralwidget);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(20, 210, 121, 34));
        label_16->setFont(font1);
        label_16->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 75 italic 20pt \"Ubuntu Condensed\";"));
        label_17 = new QLabel(centralwidget);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(160, 210, 121, 34));
        label_17->setFont(font1);
        label_17->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 75 italic 20pt \"Ubuntu Condensed\";"));
        tempEdit_2 = new QLabel(centralwidget);
        tempEdit_2->setObjectName(QString::fromUtf8("tempEdit_2"));
        tempEdit_2->setGeometry(QRect(730, 560, 301, 34));
        tempEdit_2->setFont(font1);
        tempEdit_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 75 italic 20pt \"Ubuntu Condensed\";"));
        MainWindow->setCentralWidget(centralwidget);
        listView->raise();
        pushButton->raise();
        pushButton_2->raise();
        label_3->raise();
        label_4->raise();
        label_ir->raise();
        label_light->raise();
        label_dis->raise();
        label_tmp->raise();
        label_hum->raise();
        pushButton_4->raise();
        label_7->raise();
        label_people->raise();
        label_8->raise();
        label_air->raise();
        pushButton_5->raise();
        toggleButton->raise();
        recvEdit->raise();
        sendEdit->raise();
        portEdit->raise();
        IPEdit->raise();
        openBt->raise();
        closeBt->raise();
        sendBt->raise();
        label_9->raise();
        label_11->raise();
        label_12->raise();
        label_13->raise();
        label_time_1->raise();
        label_time_2->raise();
        verticalLayoutWidget->raise();
        verticalLayoutWidget_2->raise();
        textEdit->raise();
        label_10->raise();
        label_15->raise();
        pushButton_heart->raise();
        pushButton_6->raise();
        tempEdit->raise();
        label_14->raise();
        playComfortButton->raise();
        playSleepButton->raise();
        label_16->raise();
        label_17->raise();
        tempEdit_2->raise();

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButton->setText(QString());
        pushButton_2->setText(QString());
        label_3->setText(QCoreApplication::translate("MainWindow", "\345\246\202\345\216\225\346\254\241\346\225\260:", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\345\205\211\347\205\247\357\274\232", nullptr));
        label_ir->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_light->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_dis->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_tmp->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_hum->setText(QCoreApplication::translate("MainWindow", "75", nullptr));
        pushButton_4->setText(QString());
        label_7->setText(QCoreApplication::translate("MainWindow", "\351\243\237\347\211\251\351\207\215\351\207\217:", nullptr));
        label_people->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "\351\245\256\346\260\264\346\260\264\344\275\215:", nullptr));
        label_air->setText(QCoreApplication::translate("MainWindow", "300", nullptr));
        pushButton_5->setText(QString());
        toggleButton->setText(QString());
        openBt->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200", nullptr));
        closeBt->setText(QCoreApplication::translate("MainWindow", "\345\205\263\351\227\255", nullptr));
        sendBt->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "\346\216\245\346\224\266", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "\347\253\257\345\217\243", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "IP", nullptr));
        label_time_1->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        label_time_2->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        label_10->setText(QString());
        label_15->setText(QString());
        pushButton_heart->setText(QString());
        pushButton_6->setText(QString());
        tempEdit->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "\344\275\223\346\270\251\357\274\232", nullptr));
        playComfortButton->setText(QString());
        playSleepButton->setText(QString());
        label_16->setText(QCoreApplication::translate("MainWindow", "\345\256\211\346\205\260\351\237\263\344\271\220", nullptr));
        label_17->setText(QCoreApplication::translate("MainWindow", "\347\235\241\347\234\240\351\237\263\344\271\220", nullptr));
        tempEdit_2->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
