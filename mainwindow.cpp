#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "LoginDialog.h"
#include "VideoPlayer.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    LoginDialog loginDialog;
    videoPlayer player;
    //if(loginDialog.exec() == QDialog::Accepted) {
        // 登录成功后的主界面逻辑
        player.show();
    //}
}

MainWindow::~MainWindow()
{
    delete ui;
}
