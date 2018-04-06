#include "chatwidget.h"
#include <QHostAddress>
#include <QMessageBox>
#include <QPushButton>
#include <QDateTime>
#include <QDebug>

#define MAXINFORLEN 50
#define MAXMESSLEN 150

ChatWidget::ChatWidget(QString username, QString password) : tcpSocket(new QTcpSocket),
    username(username), password(password), showMess(new QTextEdit(this)), inputMess(new QLineEdit(this)),
    sendMess(new QPushButton("发送", this)), closeButton(new QPushButton("关闭", this))
{
    showMess->setGeometry(40, 50, 620, 450);
    inputMess->setGeometry(140, 510, 420, 30);
    sendMess->setGeometry(570, 510, 90, 30);
    closeButton->setGeometry(620, 10, 50, 20);

    tcpSocket->connectToHost(QHostAddress("192.168.2.112"), 7777);
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectError()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    connect(sendMess, SIGNAL(clicked(bool)), this, SLOT(sendMessage()));
    connect(inputMess, SIGNAL(returnPressed()), this, SLOT(sendMessage()));
    connect(closeButton, SIGNAL(clicked(bool)), this, SLOT(hide()));

    mp.clear();

    inBlock.clear();
    totalBytes = 0;
}

void ChatWidget::tryLogin()
{
    QByteArray outBlock;
    QDataStream out(&outBlock, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_5);
    out << (qint64)0 << TRYLOGIN << username << password;
    out.device()->seek(0);
    out << (qint64)outBlock.size();
    tcpSocket->write(outBlock);
}

void ChatWidget::sendMessage()
{
    if(inputMess->text().isEmpty())
    {
        QMessageBox::warning(this, "发送失败", "发送内容不能为空");
        return;
    }
    QByteArray outBlock;
    QDataStream out(&outBlock, QIODevice::ReadWrite);
    out << (qint64)0 << NEWMESSAGE << inputMess->text();
    out.device()->seek(0);
    out << (qint64)outBlock.size();
    if(outBlock.size() > MAXMESSLEN)
    {
        QMessageBox::warning(this, "发送失败", "发送内容过长");
        return;
    }
    tcpSocket->write(outBlock);
    inputMess->clear();
}

void ChatWidget::connected()
{
    tryLogin();
}

void ChatWidget::connectError()
{
    QMessageBox::warning(NULL, "连接网络失败", tcpSocket->errorString());
    exit(0);
    tcpSocket->abort();
    tcpSocket->connectToHost(QHostAddress("192.168.2.112"), 7777);
}

void ChatWidget::readyRead()
{
    inBlock.append(tcpSocket->readAll());
    if(totalBytes == 0 && inBlock.size() >= (int)(sizeof(qint64) + sizeof(int)))
    {
        QDataStream in(&inBlock, QIODevice::ReadWrite);
        in.setVersion(QDataStream::Qt_5_5);
        in >> totalBytes;
        in >> messageType;
        inBlock = in.device()->readAll();
    }
    if(inBlock.size() >= totalBytes - 12)
    {
        QDataStream in(&inBlock, QIODevice::ReadWrite);
        in.setVersion(QDataStream::Qt_5_5);
        switch (messageType) {
        case NEWMESSAGE:
            newMessageComing(in);
            break;
        case TRYLOGIN:
            loginResultComing(in);
            break;
        case NEWCHAT:
            newChatComing(in);
            break;
        default:
            break;
        }
        inBlock = in.device()->readAll();
        totalBytes = 0;
        if(inBlock.size() >= (int)(sizeof(qint64) + sizeof(int)))
            readyRead();
    }
}

void ChatWidget::newMessageComing(QDataStream &in)
{
    QDateTime time;
    QString name, mess;
    in >> name >> time >> mess;
    publicMess+=QString("%1 %2:%3\n").arg(time.toString("yyyy-MM-dd hh:mm:ss")).arg(name).arg(mess);
    showMess->setPlainText(publicMess);
}

void ChatWidget::loginResultComing(QDataStream &in)
{
    bool result;
    in >> result;
    if(!result)
    {
        QMessageBox::warning(NULL, "登录失败", "连接聊天服务器失败");
        exit(0);
    }
    int count;
    in >> count;
    while(count--)
        newMessageComing(in);
}

void ChatWidget::newChatComing(QDataStream &in)
{
    QString newMessage, username;
    QDateTime time;
    in >> newMessage >> username >> time;
    showMess->append(time.toString() + " " + username + ":" + newMessage);
}

ChatWidget::~ChatWidget()
{

}
