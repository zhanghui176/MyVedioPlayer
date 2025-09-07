/********************************************************************************
** Form generated from reading UI file 'VideoPlayer.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIDEOPLAYER_H
#define UI_VIDEOPLAYER_H

#include <OpenGLVideoWidget.h>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_videoPlayer
{
public:
    QWidget *centralwidget;
    QPushButton *openButton;
    QSlider *ProgressSlider;
    QPushButton *pushButton;
    QPushButton *PlayPause;
    QSlider *volumeSlider;
    OpenGLVideoWidget *VedioWidget;
    QLabel *durationLabel;
    QMenuBar *menubar;
    QMenu *menunew;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *videoPlayer)
    {
        if (videoPlayer->objectName().isEmpty())
            videoPlayer->setObjectName("videoPlayer");
        videoPlayer->resize(702, 567);
        centralwidget = new QWidget(videoPlayer);
        centralwidget->setObjectName("centralwidget");
        openButton = new QPushButton(centralwidget);
        openButton->setObjectName("openButton");
        openButton->setGeometry(QRect(11, 11, 75, 23));
        ProgressSlider = new QSlider(centralwidget);
        ProgressSlider->setObjectName("ProgressSlider");
        ProgressSlider->setGeometry(QRect(10, 440, 611, 22));
        ProgressSlider->setOrientation(Qt::Horizontal);
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(110, 490, 75, 23));
        PlayPause = new QPushButton(centralwidget);
        PlayPause->setObjectName("PlayPause");
        PlayPause->setGeometry(QRect(10, 490, 75, 23));
        volumeSlider = new QSlider(centralwidget);
        volumeSlider->setObjectName("volumeSlider");
        volumeSlider->setGeometry(QRect(250, 490, 51, 22));
        volumeSlider->setOrientation(Qt::Horizontal);
        VedioWidget = new OpenGLVideoWidget(centralwidget);
        VedioWidget->setObjectName("VedioWidget");
        VedioWidget->setGeometry(QRect(10, 60, 681, 371));
        durationLabel = new QLabel(centralwidget);
        durationLabel->setObjectName("durationLabel");
        durationLabel->setGeometry(QRect(630, 440, 61, 19));
        videoPlayer->setCentralWidget(centralwidget);
        menubar = new QMenuBar(videoPlayer);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 702, 21));
        menunew = new QMenu(menubar);
        menunew->setObjectName("menunew");
        videoPlayer->setMenuBar(menubar);
        statusbar = new QStatusBar(videoPlayer);
        statusbar->setObjectName("statusbar");
        videoPlayer->setStatusBar(statusbar);

        menubar->addAction(menunew->menuAction());

        retranslateUi(videoPlayer);

        QMetaObject::connectSlotsByName(videoPlayer);
    } // setupUi

    void retranslateUi(QMainWindow *videoPlayer)
    {
        videoPlayer->setWindowTitle(QCoreApplication::translate("videoPlayer", "MainWindow", nullptr));
        openButton->setText(QCoreApplication::translate("videoPlayer", "open", nullptr));
        pushButton->setText(QCoreApplication::translate("videoPlayer", "PushButton", nullptr));
        PlayPause->setText(QCoreApplication::translate("videoPlayer", "PlayPause", nullptr));
        durationLabel->setText(QCoreApplication::translate("videoPlayer", "TextLabel", nullptr));
        menunew->setTitle(QCoreApplication::translate("videoPlayer", "My VedioPlayer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class videoPlayer: public Ui_videoPlayer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIDEOPLAYER_H
