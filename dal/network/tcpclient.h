#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "mainview.h"
#include "views/businessdialog.h"

#include <QTcpSocket>
#include <QByteArray>

class TcpClient:public QTcpSocket {
    Q_OBJECT

public:
    TcpClient(MainView *parent);

signals:
    void getLoginResult(QDataStream&);
    void getPlantResult(QDataStream&);
    void getBusinessResult(QDataStream&);
    void getHarvestResult(QDataStream&);
    void getSpadResult(QDataStream&);
    void getStatusChangeResult(QDataStream&);
    void getReclaResult(QDataStream&);
    void getFertilizeResult(QDataStream&);
    void getUpdateResult(QDataStream&);

public slots:
    void readyRead();
    void connectError();
    void connectSuccess();
    void sendRequest(QByteArray outBlock) {
        qDebug() << "Send a request, Content is: " << outBlock;
        write(outBlock);
    }

private:
    MainView* parent;
    qint64 totalBytes;
    QByteArray inBlock;
    int messageType;

    int retryNum = 3;
};


#endif
