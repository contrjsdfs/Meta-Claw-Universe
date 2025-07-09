/********************************************************************************
** Form generated from reading UI file 'heart.ui'
**
** Created by: Qt User Interface Compiler version 5.15.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HEART_H
#define UI_HEART_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_heart
{
public:
    QPushButton *pushButton;
    QPushButton *pushButton_8;
    QPushButton *pushButton_7;
    QPushButton *pushButton_3;
    QListView *listView;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QPushButton *startButton;
    QPushButton *stopButton;
    QTimeEdit *timeEdit1;
    QTimeEdit *timeEdit2;
    QTimeEdit *timeEdit3;
    QLabel *label_9;
    QLabel *tempEdit;
    QLabel *tempEdit_2;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;

    void setupUi(QWidget *heart)
    {
        if (heart->objectName().isEmpty())
            heart->setObjectName(QString::fromUtf8("heart"));
        heart->resize(1024, 600);
        pushButton = new QPushButton(heart);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(60, 60, 71, 71));
        pushButton->setStyleSheet(QString::fromUtf8("border-image: url(:/\346\211\213\345\212\250.png);"));
        pushButton_8 = new QPushButton(heart);
        pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_8->setGeometry(QRect(260, 70, 89, 81));
        pushButton_8->setStyleSheet(QString::fromUtf8("border-image: url(:/\345\212\240\347\203\255.png);"));
        pushButton_8->setCheckable(true);
        pushButton_7 = new QPushButton(heart);
        pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));
        pushButton_7->setGeometry(QRect(260, 340, 89, 81));
        pushButton_7->setStyleSheet(QString::fromUtf8("border-image: url(:/\351\243\216\346\211\207(2).png);"));
        pushButton_7->setCheckable(true);
        pushButton_3 = new QPushButton(heart);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(50, 330, 91, 81));
        pushButton_3->setStyleSheet(QString::fromUtf8("border-image: url(:/\345\205\263\351\233\276\345\214\226.png);"));
        pushButton_3->setCheckable(true);
        listView = new QListView(heart);
        listView->setObjectName(QString::fromUtf8("listView"));
        listView->setGeometry(QRect(-20, -10, 1051, 621));
        listView->setStyleSheet(QString::fromUtf8("border-image: url(:/\350\203\214\346\231\257\345\233\276.png);"));
        label = new QLabel(heart);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 150, 121, 71));
        label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 75 italic 20pt \"Lohit Tamil\";"));
        label_2 = new QLabel(heart);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(260, 160, 121, 71));
        label_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 75 italic 20pt \"Lohit Tamil\";"));
        label_3 = new QLabel(heart);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(270, 430, 121, 71));
        label_3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 75 italic 20pt \"Lohit Tamil\";"));
        label_4 = new QLabel(heart);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(40, 430, 121, 71));
        label_4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 75 italic 20pt \"Lohit Tamil\";"));
        startButton = new QPushButton(heart);
        startButton->setObjectName(QString::fromUtf8("startButton"));
        startButton->setGeometry(QRect(620, 420, 111, 61));
        startButton->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(28, 113, 216);\n"
"font: 75 italic 20pt \"Lohit Tamil\";"));
        stopButton = new QPushButton(heart);
        stopButton->setObjectName(QString::fromUtf8("stopButton"));
        stopButton->setGeometry(QRect(790, 420, 111, 61));
        stopButton->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(28, 113, 216);\n"
"font: 75 italic 20pt \"Lohit Tamil\";"));
        timeEdit1 = new QTimeEdit(heart);
        timeEdit1->setObjectName(QString::fromUtf8("timeEdit1"));
        timeEdit1->setEnabled(false);
        timeEdit1->setGeometry(QRect(640, 90, 200, 60));
        timeEdit1->setStyleSheet(QString::fromUtf8("font: 75 16pt \"Ubuntu\";"));
        timeEdit1->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        timeEdit1->setCorrectionMode(QAbstractSpinBox::CorrectToNearestValue);
        timeEdit1->setCalendarPopup(false);
        timeEdit1->setTime(QTime(0, 0, 0));
        timeEdit2 = new QTimeEdit(heart);
        timeEdit2->setObjectName(QString::fromUtf8("timeEdit2"));
        timeEdit2->setGeometry(QRect(640, 190, 200, 60));
        timeEdit2->setStyleSheet(QString::fromUtf8("font: 75 16pt \"Ubuntu\";"));
        timeEdit3 = new QTimeEdit(heart);
        timeEdit3->setObjectName(QString::fromUtf8("timeEdit3"));
        timeEdit3->setGeometry(QRect(640, 290, 200, 60));
        timeEdit3->setStyleSheet(QString::fromUtf8("font: 75 16pt \"Ubuntu\";"));
        label_9 = new QLabel(heart);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(640, 0, 331, 71));
        label_9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 63 42pt \"URW Bookman\";\n"
"font: 63 66pt \"URW Bookman\";\n"
"font: 95 italic 20pt \"Lohit Tamil\";"));
        tempEdit = new QLabel(heart);
        tempEdit->setObjectName(QString::fromUtf8("tempEdit"));
        tempEdit->setGeometry(QRect(730, 520, 301, 34));
        QFont font;
        font.setFamily(QString::fromUtf8("Ubuntu Condensed"));
        font.setPointSize(20);
        font.setBold(false);
        font.setItalic(true);
        font.setWeight(9);
        tempEdit->setFont(font);
        tempEdit->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 75 italic 20pt \"Ubuntu Condensed\";"));
        tempEdit_2 = new QLabel(heart);
        tempEdit_2->setObjectName(QString::fromUtf8("tempEdit_2"));
        tempEdit_2->setGeometry(QRect(630, 520, 111, 34));
        tempEdit_2->setFont(font);
        tempEdit_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 75 italic 20pt \"Ubuntu Condensed\";"));
        widget = new QWidget(heart);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(880, 80, 81, 301));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label_5 = new QLabel(widget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 75 italic 20pt \"Lohit Tamil\";"));

        verticalLayout->addWidget(label_5);

        label_6 = new QLabel(widget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 75 italic 20pt \"Lohit Tamil\";"));

        verticalLayout->addWidget(label_6);

        label_7 = new QLabel(widget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 75 italic 20pt \"Lohit Tamil\";"));

        verticalLayout->addWidget(label_7);

        listView->raise();
        pushButton->raise();
        pushButton_8->raise();
        pushButton_7->raise();
        pushButton_3->raise();
        label->raise();
        label_2->raise();
        label_3->raise();
        label_4->raise();
        label_5->raise();
        label_6->raise();
        label_7->raise();
        startButton->raise();
        stopButton->raise();
        timeEdit1->raise();
        timeEdit2->raise();
        timeEdit3->raise();
        label_9->raise();
        tempEdit->raise();
        tempEdit_2->raise();

        retranslateUi(heart);

        QMetaObject::connectSlotsByName(heart);
    } // setupUi

    void retranslateUi(QWidget *heart)
    {
        heart->setWindowTitle(QCoreApplication::translate("heart", "Form", nullptr));
        pushButton->setText(QString());
        pushButton_8->setText(QString());
        pushButton_7->setText(QString());
        pushButton_3->setText(QString());
        label->setText(QCoreApplication::translate("heart", "\344\272\272\345\267\245\346\216\247\345\210\266", nullptr));
        label_2->setText(QCoreApplication::translate("heart", "\345\212\240\347\203\255\345\231\250", nullptr));
        label_3->setText(QCoreApplication::translate("heart", "\351\243\216\346\211\207", nullptr));
        label_4->setText(QCoreApplication::translate("heart", "\351\233\276\345\214\226\345\231\250", nullptr));
        startButton->setText(QCoreApplication::translate("heart", "start", nullptr));
        stopButton->setText(QCoreApplication::translate("heart", "stop", nullptr));
#if QT_CONFIG(whatsthis)
        timeEdit3->setWhatsThis(QCoreApplication::translate("heart", "<html><head/><body><p><img src=\":/1.png\"/></p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
        label_9->setText(QCoreApplication::translate("heart", "\345\256\232\346\227\266\345\226\202\351\243\237\345\274\200\347\201\257", nullptr));
        tempEdit->setText(QCoreApplication::translate("heart", "\345\267\262\345\205\263\351\227\255\345\256\232\346\227\266\345\226\202\351\243\237", nullptr));
        tempEdit_2->setText(QCoreApplication::translate("heart", "\347\212\266\346\200\201\357\274\232", nullptr));
        label_5->setText(QCoreApplication::translate("heart", "\346\227\251\344\270\212", nullptr));
        label_6->setText(QCoreApplication::translate("heart", "\344\270\255\345\215\210", nullptr));
        label_7->setText(QCoreApplication::translate("heart", "\346\231\232\344\270\212", nullptr));
    } // retranslateUi

};

namespace Ui {
    class heart: public Ui_heart {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HEART_H
