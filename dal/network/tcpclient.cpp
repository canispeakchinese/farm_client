#include "tcpclient.h"

#include <QDataStream>
#include <QIODevice>
#include <QMessageBox>
#include <QHostAddress>

TcpClient::TcpClient(): QTcpSocket(new QTcpSocket()) {
    totalBytes = 0;
    inBlock.resize(0);
    connectToHost(QHostAddress("192.168.199.183"), 6666);
}

void TcpClient::readyRead() {
    if(!bytesAvailable())
        return;
    inBlock.append(readAll());
    if(totalBytes == 0 && inBlock.size() >= (int)(sizeof(qint64) + sizeof(int))) {
        QDataStream in(&inBlock, QIODevice::ReadWrite);
        in.setVersion(QDataStream::Qt_5_5);
        in >> totalBytes;
        in >> messageType;
        inBlock = in.device()->readAll();
    }
    if(inBlock.size() >= totalBytes - 12) {
        QDataStream in(&inBlock, QIODevice::ReadWrite);
        in.setVersion(QDataStream::Qt_5_5);
        switch (messageType) {
        case 1:
            emit getLoginResult(in);
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
