#include "tcpclient.h"

#include <QDataStream>
#include <QIODevice>
#include <QMessageBox>
#include <QHostAddress>

TcpClient::TcpClient(MainView* parent): QTcpSocket(new QTcpSocket()), parent(parent) {
    totalBytes = 0;
    inBlock.resize(0);

    connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectError()));
    connect(this, SIGNAL(connected()), this, SLOT(connectSuccess()));
    connect(this, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void TcpClient::start() {
    // 连接失败直接报错
    qDebug() << "Start connect to server";

    connectToHost(QHostAddress(ipAddress), 6666);
    if(waitForConnected(1000) == false) {
        connectError();
    }
}

void TcpClient::connectError() {
    qDebug() << "TcpClient Connect Error: " << errorString() << ", can retry num: " << retryNum;
    if(retryNum > 0) {
        MainView::tcpMutex.lock();
        retryNum--;
        MainView::tcpMutex.unlock();
        QMessageBox::warning(parent, "连接网络失败", errorString());
        connectToHost(QHostAddress(ipAddress), 6666);
        if(waitForConnected(1000) == false) {
            connectError();
        }
    } else {
        exit(-1);
    }
}

void TcpClient::connectSuccess() {
    qDebug() << "TcpClient Connect Success, And Set Connect Retry Num to 3";
    MainView::tcpMutex.lock();
    retryNum = 3;
    MainView::tcpMutex.unlock();
}

void TcpClient::readyRead() {
    if(!bytesAvailable())
        return;
    inBlock.append(readAll());
    while(true) {
        if(totalBytes == 0) {
            if(inBlock.size() >= (int)(sizeof(qint64) + sizeof(int))) {
                QDataStream in(&inBlock, QIODevice::ReadWrite);
                in.setVersion(QDataStream::Qt_5_5);
                in >> totalBytes;
                in >> messageType;
                inBlock = in.device()->readAll();
            } else {
                break;
            }
        }
        if(inBlock.size() >= totalBytes - 12) {
            QDataStream all(&inBlock, QIODevice::ReadWrite);
            all.setVersion(QDataStream::Qt_5_5);
            QByteArray message = all.device()->read(totalBytes - 12);

            QDataStream in(&message, QIODevice::ReadWrite);
            in.setVersion(QDataStream::Qt_5_5);
            switch (messageType) {
            case 1:
                emit getLoginResult(in);
                break;
            case 2:
                emit getSignResult(in);
                break;
            case 3:
                emit getUpdateResult(in);
                break;
            case 4:
                emit getPlantResult(in);
                break;
            case 5:
                emit getBusinessResult(in);
                break;
            case 6:
                emit getSpadResult(in);
                break;
            case 7:
                emit getHarvestResult(in);
                break;
            case 8:
                emit getStatusChangeResult(in);
                break;
            case 9:
                emit getReclaResult(in);
                break;
            case 10:
                emit getFertilizeResult(in);
                break;
            default:
                break;
            }
            inBlock = all.device()->readAll();
            totalBytes = 0;
        }
    }
}
