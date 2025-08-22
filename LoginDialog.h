#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QDialog>

class LoginDialog: public QDialog
{
    Q_OBJECT;
public:
    explicit LoginDialog(QWidget *parent = nullptr);
private slots:
    void onClickLoginButton();
    void onClickCancelButton();
private:
    QLabel *usenameLabel_;
    QLabel *passwordLabel_;
    QLineEdit *usenameEdit_;
    QLineEdit *passwordEdit_;
    QPushButton *loginButton_;
    QPushButton *cancelButton_;
    QPushButton *registerButton_;
};

#endif // LOGINDIALOG_H
