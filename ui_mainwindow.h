/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox_2;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_4;
    QLabel *label;
    QLineEdit *FolderEdit;
    QRadioButton *radioButton;
    QSpinBox *spinBox_inter_epoch;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *Duration;
    QComboBox *comboBox;
    QSpinBox *repeats;
    QLabel *label_3;
    QLabel *label_5;
    QLabel *label_6;
    QSpinBox *spinBox_rectime;
    QPushButton *pushButton_4;
    QPushButton *pushButton_2;
    QLabel *label_7;
    QSpinBox *spinBox_waiting_time;
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox;
    QPushButton *pushButton;
    QPushButton *pushButton_5;
    QGroupBox *groupBox_3;
    QPushButton *pushButton_3;
    QProgressBar *progressBar;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(582, 406);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setMinimumSize(QSize(350, 0));
        gridLayoutWidget_2 = new QWidget(groupBox_2);
        gridLayoutWidget_2->setObjectName(QString::fromUtf8("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(10, 30, 336, 278));
        gridLayout_4 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(gridLayoutWidget_2);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_4->addWidget(label, 1, 0, 1, 1);

        FolderEdit = new QLineEdit(gridLayoutWidget_2);
        FolderEdit->setObjectName(QString::fromUtf8("FolderEdit"));

        gridLayout_4->addWidget(FolderEdit, 1, 1, 1, 2);

        radioButton = new QRadioButton(gridLayoutWidget_2);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));

        gridLayout_4->addWidget(radioButton, 0, 0, 1, 3);

        spinBox_inter_epoch = new QSpinBox(gridLayoutWidget_2);
        spinBox_inter_epoch->setObjectName(QString::fromUtf8("spinBox_inter_epoch"));
        spinBox_inter_epoch->setValue(20);

        gridLayout_4->addWidget(spinBox_inter_epoch, 5, 2, 1, 1);

        label_2 = new QLabel(gridLayoutWidget_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_4->addWidget(label_2, 4, 0, 1, 2);

        label_4 = new QLabel(gridLayoutWidget_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_4->addWidget(label_4, 5, 0, 1, 2);

        Duration = new QLabel(gridLayoutWidget_2);
        Duration->setObjectName(QString::fromUtf8("Duration"));

        gridLayout_4->addWidget(Duration, 9, 1, 1, 1);

        comboBox = new QComboBox(gridLayoutWidget_2);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        gridLayout_4->addWidget(comboBox, 7, 2, 1, 1);

        repeats = new QSpinBox(gridLayoutWidget_2);
        repeats->setObjectName(QString::fromUtf8("repeats"));
        repeats->setValue(1);

        gridLayout_4->addWidget(repeats, 4, 2, 1, 1);

        label_3 = new QLabel(gridLayoutWidget_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_4->addWidget(label_3, 7, 0, 1, 2);

        label_5 = new QLabel(gridLayoutWidget_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_4->addWidget(label_5, 9, 0, 1, 1);

        label_6 = new QLabel(gridLayoutWidget_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_4->addWidget(label_6, 8, 0, 1, 2);

        spinBox_rectime = new QSpinBox(gridLayoutWidget_2);
        spinBox_rectime->setObjectName(QString::fromUtf8("spinBox_rectime"));
        spinBox_rectime->setValue(60);

        gridLayout_4->addWidget(spinBox_rectime, 8, 2, 1, 1);

        pushButton_4 = new QPushButton(gridLayoutWidget_2);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));

        gridLayout_4->addWidget(pushButton_4, 10, 0, 1, 1);

        pushButton_2 = new QPushButton(gridLayoutWidget_2);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        gridLayout_4->addWidget(pushButton_2, 10, 2, 1, 1);

        label_7 = new QLabel(gridLayoutWidget_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_4->addWidget(label_7, 6, 0, 1, 2);

        spinBox_waiting_time = new QSpinBox(gridLayoutWidget_2);
        spinBox_waiting_time->setObjectName(QString::fromUtf8("spinBox_waiting_time"));

        gridLayout_4->addWidget(spinBox_waiting_time, 6, 2, 1, 1);


        horizontalLayout->addWidget(groupBox_2);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        gridLayout_2->setContentsMargins(-1, -1, 0, -1);
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(120, 100));
        groupBox->setMaximumSize(QSize(16777215, 16777215));
        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(10, 30, 98, 27));
        pushButton_5 = new QPushButton(groupBox);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setGeometry(QRect(10, 70, 99, 27));

        gridLayout_2->addWidget(groupBox, 0, 0, 1, 1);

        groupBox_3 = new QGroupBox(centralwidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setMinimumSize(QSize(120, 100));
        groupBox_3->setMaximumSize(QSize(16777215, 16777215));
        pushButton_3 = new QPushButton(groupBox_3);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(10, 30, 98, 27));

        gridLayout_2->addWidget(groupBox_3, 1, 0, 1, 1);


        horizontalLayout->addLayout(gridLayout_2);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        progressBar = new QProgressBar(centralwidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(100);

        gridLayout->addWidget(progressBar, 1, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 582, 23));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Rec2Track", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Settings", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Folder", 0, QApplication::UnicodeUTF8));
        radioButton->setText(QApplication::translate("MainWindow", "Visual Stimulation", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Repeats", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Inter epoch time (s)", 0, QApplication::UnicodeUTF8));
        Duration->setText(QApplication::translate("MainWindow", "0 s", 0, QApplication::UnicodeUTF8));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Tom", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Rachel", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Dominic", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Tom Shallcross", 0, QApplication::UnicodeUTF8)
        );
        label_3->setText(QApplication::translate("MainWindow", "User", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "Barrage duration", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "Recording time (m)", 0, QApplication::UnicodeUTF8));
        pushButton_4->setText(QApplication::translate("MainWindow", "Generate barrage", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("MainWindow", "Record", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "Waiting time (s)", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("MainWindow", "Display", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("MainWindow", "Show camera", 0, QApplication::UnicodeUTF8));
        pushButton_5->setText(QApplication::translate("MainWindow", "Run barrage", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Analysis", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("MainWindow", "Track", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
