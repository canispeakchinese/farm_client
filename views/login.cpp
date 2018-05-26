#include "login.h"
#include "showinforgroup.h"

#include <QTcpSocket>
#include <QDataStream>
#include <QTimer>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QDataStream>
#include <QHostAddress>

Login::Login(QWidget * parent) : QWidget(parent), userName(new QLabel(this)),
    passWord(new QLabel(this)), inputName(new QLineEdit(this)),
    inputWord(new QLineEdit(this)), login(new QPushButton(this)), sign(new QPushButton(this))
{
    //setWindowFlags(Qt::FramelessWindowHint);
    setMaximumSize(500, 300);
    setMinimumSize(500, 300);

    userName->setGeometry(50, 120, 180, 30);
    userName->setText("帐号");
    inputName->setGeometry(270,120, 180, 30);
    passWord->setGeometry(50, 160, 180, 30);
    inputWord->setGeometry(270, 160, 180, 30);
    inputWord->setEchoMode(QLineEdit::Password  );
    passWord->setText("密码");
    login->setGeometry(50, 200, 180, 30);
    login->setText("登录");
    sign->setGeometry(270, 200, 180, 30);
    sign->setText("注册");

    connect(login, SIGNAL(clicked(bool)), this, SLOT(tryLogin()));
    connect(sign, SIGNAL(clicked(bool)), this, SLOT(trySign()));
    connect(inputWord, SIGNAL(returnPressed()), this, SLOT(tryLogin()));
}

Login::~Login()
{
}

void Login::tryLogin()
{
    MainView::username = inputName->text();
    MainView::password = inputWord->text();

    QByteArray outBlock;
    QDataStream out(&outBlock, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_5);
    out << (qint64)(0);
    out << 1;//LogIn;
    out << MainView::username << MainView::password;
    out.device()->seek(0);
    out << (qint64)outBlock.size();
    emit sendLoginRequest(outBlock);
}

void Login::trySign()
{
    if(inputName->text().length() < 6 || inputWord->text().length() < 6)
    {
        QMessageBox::warning(this, "注册失败", "用户名或密码过短");
        return;
    }

    MainView::username = inputName->text();
    MainView::password = inputWord->text();

    QByteArray outBlock;
    QDataStream out(&outBlock, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_5);
    out << (qint64)(0);
    out << SignIn;
    out << MainView::username << MainView::password;
    out.device()->seek(0);
    out << (qint64)outBlock.size();
    emit sendLoginRequest(outBlock);
}

void Login::getSignResult(QDataStream &in) {
    int errno;
    in >> errno;
    qDebug() << "Sign errno is " << errno;
    switch (errno) {
    case 0:
        emit logInSuccess(in);
        this->close();
        break;
    case 1:
        QMessageBox::warning(this, "注册失败", "用户名已存在，请重试");
        break;
    default:
        break;
    }
}

void Login::getLoginResult(QDataStream &in)
{
    bool ok;
    in >> ok;
    if(!ok)
    {
        int type;
        in >> type;
        switch (type) {
        case 0:
            QMessageBox::warning(this, "登录失败", "帐号或密码错误,请重试");
            break;
        case 1:
            QMessageBox::warning(this, "登录失败", "该帐号已在别处登录，请先下线");
        default:
            break;
        }
    } else {
        emit logInSuccess(in);
        this->close();
    }
}

