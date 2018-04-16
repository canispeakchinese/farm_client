#include "mainview.h"
#include "dal/baseview/tool.h"
#include "dal/network/tcpclient.h"
#include "views/login.h"
#include "views/soil.h"
#include "views/store.h"
#include "views/shop.h"
#include "views/showfriendgroup.h"
#include "views/showinforgroup.h"
#include "views/packgroup.h"
#include "views/showscenegroup.h"
#include "views/chatwidget.h"

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QGraphicsWidget>
#include <QIcon>
#include <QMessageBox>
#include <QTcpSocket>
#include <QDebug>
#include <QHostAddress>
#include <QDataStream>
#include <QTextBlockFormat>
#include <QTextCursor>

Good MainView::good;
UserInfo MainView::person;
ToolType MainView::toolType = Empty;
QString MainView::username = "";
QString MainView::password = "";
QMutex MainView::updateMutex;
QMutex MainView::statusMutex;
QMutex MainView::tcpMutex;
QMutex MainView::userInfoMutex;
QMutex MainView::goodMutex;

MainView::MainView(QGraphicsView *parent) : QGraphicsView(parent), tcpClient(new TcpClient(this)), login(new Login()) {
    setMouseTracking(true);

    setMinimumSize(1000,650);
    setMaximumSize(1000,650);

    source = MainViewSource;

    // 登陆相关
    connect(tcpClient, SIGNAL(connected()), login, SLOT(show()));
    connect(login, SIGNAL(sendLoginRequest(QByteArray)), tcpClient, SLOT(sendRequest(QByteArray)));
    connect(tcpClient, SIGNAL(getLoginResult(QDataStream&)), login, SLOT(getLoginResult(QDataStream&)));
    connect(login, SIGNAL(logInSuccess(QDataStream&)), this, SLOT(logInSuccess(QDataStream&)));
    // 获取某一个模块的最新数据（如土地状态等）
    connect(this, SIGNAL(sendUpdateRequest(QByteArray)), tcpClient, SLOT(sendRequest(QByteArray)));
    connect(tcpClient, SIGNAL(getUpdateResult(QDataStream&)), this, SLOT(getUpdateResult(QDataStream&)));
}

QByteArray MainView::createUpdateRequest(int request) {
    QByteArray outBlock;
    QDataStream out(&outBlock, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_5);
    out << qint64(0) << 3 << request;
    out.device()->seek(0);
    out << (qint64)outBlock.size();
    return outBlock;
}

void MainView::logInSuccess(QDataStream &in) {
    QString face;
    int id, level, exp, money;
    in >> id >> face >> level >> exp >> money;
    person = UserInfo(MainView::username, QString(":face/image/face/%1-1.GIF").arg(face), level, exp, money);
    qDebug() << "LogIn Success, UserInfo: " << person.toString();
    initMainView();

    emit sendUpdateRequest(createUpdateRequest((1<<10)-1));
}

void MainView::initMainView() {
    scene = new QGraphicsScene;

    scene->setSceneRect(5,5,990,640);
    setScene(scene);
    scene->setBackgroundBrush(QPixmap(":background/image/background/1.png").scaled(size()));

    showstatus = new QGraphicsPixmapItem();
    scene->addItem(showstatus);
    showstatus->setZValue(1);
    showstatus->hide();

    createSoilGroup();
    createToolGroup();
    createFriendGroup();
    createInforGroup();
    createSceneGroup();
    createPackGroup();
    createChatWidget();

    statusChangeSignalConnect();
    goodChangeSignalConnect();
    moneyChangeSignalConnect();
    expChangeSignalConnect();

    networkMessageConnectSignalConnect();

    show();
}

void MainView::createSoilGroup() {
    soilgroup = new SoilGroup();
    scene->addItem(soilgroup);
    soilgroup->setPos(475,430);
    soilgroup->createSoil();

    // 初始化土地
    connect(this, SIGNAL(getSoils(QDataStream&)), soilgroup, SLOT(getSoils(QDataStream&)));
}

void MainView::createToolGroup() {
    toolgroup = new ToolGroup();
    toolgroup->createTool();
    scene->addItem(toolgroup);
    toolgroup->setPos(220,600);
}

void MainView::createFriendGroup() {
    showfriendgroup = new ShowFriendGroup();
    scene->addItem(showfriendgroup);
    showfriendgroup->setPos(1000, 325);
    connect(this, SIGNAL(getFriend(QDataStream&)), showfriendgroup, SLOT(getFriend(QDataStream&)));
}

void MainView::createInforGroup() {
    showinforgroup = new ShowInforGroup(getUserInfo());
    scene->addItem(showinforgroup);
    showinforgroup->setPos(135,80);
}

void MainView::createSceneGroup() {
    showscenegroup = new ShowSceneGroup(person.level);
    scene->addItem(showscenegroup);
    showscenegroup->createButton();
    showscenegroup->setPos(980,70);
    connect(this, SIGNAL(getGoods(QDataStream&,Business)), showscenegroup, SLOT(getGoods(QDataStream&,Business)));
}

void MainView::createPackGroup() {
    packgroup = new PackGroup(person.level);
    packgroup->setParent(this);
    packgroup->setGeometry(150, 450, 700, 220);
    connect(this, SIGNAL(getGoods(QDataStream&,Business)), packgroup, SLOT(getGoods(QDataStream&,Business)));
    packgroup->hide();
}

void MainView::createChatWidget() {
    chatWidget = new ChatWidget(username, password);
    chatWidget->setParent(this);
    chatWidget->setGeometry(150, 50, 700, 550);
    chatWidget->hide();
}

void MainView::mouseMoveEvent(QMouseEvent *event) {
    showstatus->setPos(event->x(),event->y()-30);
    QGraphicsView::mouseMoveEvent(event);
}

void MainView::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::RightButton) {
        qDebug() << "MainView Mouse Press, Status Change: " << getStatus() << "-> " << Empty;
        setStatus(Empty);
        emit statusChange(source);
    }
    QGraphicsView::mousePressEvent(event);
}

void MainView::getUpdateResult(QDataStream &in) {
    int updateRequest;
    in >> updateRequest;
    if(updateRequest & 1) {
        updateMutex.lock();
        emit getSoils(in);
    }
    if(updateRequest & 2) {
        updateMutex.lock();
        emit getFriend(in);
    }
    if(updateRequest & 4) {
        updateMutex.lock();
        emit getGoods(in, Buy);
    }
    if(updateRequest & 8) {
        updateMutex.lock();
        emit getGoods(in, Sell);
    }
    if(updateRequest & 16) {
        updateMutex.lock();
        emit getGoods(in, Use);
    }
}

void MainView::statusChangeSignalConnect() {
    connect(toolgroup, SIGNAL(statusChange(QString)), this, SLOT(statusChangeCenter(QString)));
    connect(soilgroup, SIGNAL(statusChange(QString)), this, SLOT(statusChangeCenter(QString)));
    connect(this, SIGNAL(statusChange(QString)), this, SLOT(statusChangeCenter(QString)));
    connect(packgroup, SIGNAL(statusChange(QString)), this, SLOT(statusChangeCenter(QString)));
}

void MainView::goodChangeSignalConnect() {
    connect(soilgroup, SIGNAL(goodChange(QString)), this, SLOT(goodChangeCenter(QString)));
    connect(this, SIGNAL(sendGoodChange()), packgroup, SLOT(receiveGoodChange()));

    connect(packgroup, SIGNAL(goodChange(QString)), this, SLOT(goodChangeCenter(QString)));
    connect(this, SIGNAL(sendGoodChange()), packgroup, SLOT(receiveGoodChange()));
}

void MainView::moneyChangeSignalConnect() {
    connect(soilgroup, SIGNAL(moneyChange(QString)), this, SLOT(moneyChangeCenter(QString)));
}

void MainView::expChangeSignalConnect() {
    connect(soilgroup, SIGNAL(expChange(QString)), this, SLOT(expChangeCenter(QString)));
}

void MainView::networkMessageConnectSignalConnect() {
    connect(soilgroup, SIGNAL(sendSoilRequestToServer(QByteArray)), tcpClient, SLOT(sendRequest(QByteArray)));

    connect(tcpClient, SIGNAL(getFertilizeResult(QDataStream&)), soilgroup, SLOT(getFertilizeResult(QDataStream&)));
    connect(tcpClient, SIGNAL(getPlantResult(QDataStream&)), soilgroup, SLOT(getPlantResult(QDataStream&)));
    connect(tcpClient, SIGNAL(getSpadResult(QDataStream&)), soilgroup, SLOT(getSpadResult(QDataStream&)));
    connect(tcpClient, SIGNAL(getHarvestResult(QDataStream&)), soilgroup, SLOT(getHarvestResult(QDataStream&)));
    connect(tcpClient, SIGNAL(getStatusChangeResult(QDataStream&)), soilgroup, SLOT(getStatusChangeResult(QDataStream&)));
}

void MainView::goodChangeCenter(QString source) {
    qDebug() << "Good Changed, Source: " << source;
    emit sendGoodChange();
}

void MainView::statusChangeCenter(QString source) {
    qDebug() << "Status Changed, Source: " << source;
    ToolType toolType = getStatus();
    Good good = getGood();
    if(toolType == Pack) {
        packgroup->show();
    } else if(toolType == Chat) {
        chatWidget->show();
    } else {
        if(toolType == Plant) {
            showstatus->setPixmap(QPixmap(QString("%1/seed.png").arg(good.address)));
        } else {
            showstatus->setPixmap(QPixmap(QString(":icon/image/icon/topbutton(")+QString::number(toolType)+QString(").png")));
        }
        showstatus->show();
    }
    emit sendStatusChange();
}

void MainView::expChangeCenter(QString source) {
    qDebug() << "Exp Changed, Source: " << source;
    emit sendExpChange();
}

void MainView::moneyChangeCenter(QString source) {
    qDebug() << "Money Changed, Source:" << source;
    emit sendMoneyChange();
}

MainView::~MainView()
{
}
