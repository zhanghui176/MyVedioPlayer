#include "mainwindow.h"
#include "LoginDialog.h"
#include "VideoPlayer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginDialog loginDialog;

   // if(loginDialog.exec() == QDialog::Accepted) {
        // 登录成功后的主界面逻辑
        auto player = new videoPlayer();
        player->show();
        return a.exec();
    //}
    //return a.exec();
}
