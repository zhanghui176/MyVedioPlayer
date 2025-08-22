#include "LoginDialog.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>

LoginDialog::LoginDialog(QWidget *parent): QDialog(parent)
{
    usenameLabel_ = new QLabel("用户名", this);
    passwordLabel_ = new QLabel("密码", this);
    usenameEdit_ = new QLineEdit(this);
    passwordEdit_ = new QLineEdit(this);
    passwordEdit_->setEchoMode(QLineEdit::Password);

    loginButton_ = new QPushButton("登录", this);
    cancelButton_ = new QPushButton("取消", this);
    registerButton_ = new QPushButton("注册", this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *userLayout = new QHBoxLayout();
    QHBoxLayout *pwdLayout = new QHBoxLayout();
    //QHBoxLayout *btnLayout = new QHBoxLayout();

    QGridLayout *btnLayout = new QGridLayout();

    userLayout->addWidget(usenameLabel_);
    userLayout->addWidget(usenameEdit_);
    pwdLayout->addWidget(passwordLabel_);
    pwdLayout->addWidget(passwordEdit_);
    btnLayout->addWidget(loginButton_, 0, Qt::AlignLeft);
    btnLayout->addWidget(cancelButton_, 0, Qt::AlignHCenter);
    btnLayout->addWidget(registerButton_, 0, Qt::AlignRight);

    mainLayout->addLayout(userLayout);
    mainLayout->addLayout(pwdLayout);
    mainLayout->addLayout(btnLayout);

    connect(loginButton_, &QPushButton::clicked, this, &LoginDialog::onClickLoginButton);
    connect(cancelButton_, &QPushButton::clicked, this, &LoginDialog::onClickCancelButton);
}

void LoginDialog::onClickLoginButton()
{
    if(usenameEdit_->text().isEmpty() || passwordEdit_->text().isEmpty()) {
        QMessageBox::warning(this, "警告", "用户名和密码不能为空");
    } else if(usenameEdit_->text() == "admin" && passwordEdit_->text() == "123456") {
        accept();
    } else {
        QMessageBox::critical(this, "错误", "用户名或密码错误");
    }
}

void LoginDialog::onClickCancelButton()
{
    reject();
}
