#ifndef LOGIN_H
#define LOGIN_H

#include "mainview.h"
#include <QWidget>

class QLabel;
class QLineEdit;
class QPushButton;

const qint32 LogIn = 1;
const qint32 SignIn = 2;

class Login : public QWidget
{
    Q_OBJECT

public:
    Login(QWidget * parent = 0);
    ~Login();

signals:
    void sendLoginRequest(QByteArray outBlock);
    void logInSuccess(QDataStream& in);

protected slots:
    void tryLogin();
    void trySign();
    void getLoginResult(QDataStream& in);
    void getSignResult(QDataStream& in);

private:
    QLabel * userName;
    QLabel * passWord;
    QLineEdit * inputName;
    QLineEdit * inputWord;
    QPushButton * login;
    QPushButton * sign;
};

#endif // LOGIN_H
