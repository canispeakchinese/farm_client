#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "views/businessdialog.h"

#include <QTcpSocket>
#include <QByteArray>

class TcpClient:public QTcpSocket {
    Q_OBJECT

public:
    TcpClient();

signals:
    void getLoginResult(QDataStream&);

public slots:
    void readyRead();
    void sendRequest(QByteArray outBlock) {
        write(outBlock);
    }

private:
    qint64 totalBytes;
    QByteArray inBlock;
    int messageType;
};


#endif
