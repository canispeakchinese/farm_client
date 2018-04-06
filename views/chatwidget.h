#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include "dal/baseview/basescene.h"
#include <QStringList>
#include <QGraphicsTextItem>
#include <QGraphicsItemGroup>
#include <QTcpSocket>
#include <QMap>
#include <QTextEdit>
#include <QLineEdit>
#define NEWMESSAGE 0
#define TRYLOGIN 1
#define NEWCHAT 2

class QPushButton;

class ChatWidget : public BaseScene
{
    Q_OBJECT

public:
    ChatWidget(QString username, QString password);
    void tryLogin();
    void loginResultComing(QDataStream &in);
    void newMessageComing(QDataStream &in);
    void newChatComing(QDataStream &in);
    ~ChatWidget();

public slots:
    void connected();
    void connectError();
    void readyRead();
    void sendMessage();

private:
    QTcpSocket * tcpSocket;
    QString username;
    QString password;
    QTextEdit *showMess;
    QLineEdit *inputMess;
    QPushButton *sendMess;
    QPushButton *closeButton;

    QMap<QString, QString>mp;
    QString publicMess;

    QByteArray inBlock;
    qint64 totalBytes;
    int messageType;
};

#endif // CHATWIDGET_H
