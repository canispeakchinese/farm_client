#include "mainview.h"
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

MainView::MainView(QGraphicsView *parent) : QGraphicsView(parent), tcpClient(new QTcpSocket), login(new Login()) {
    setMouseTracking(true);

    setMinimumSize(1000,650);
    setMaximumSize(1000,650);

    toolType = Empty;
    totalBytes = 0;
    inBlock.resize(0);

    tcpClient2 = new TcpClient();
    tcpClient->connectToHost(QHostAddress("192.168.199.183"), 6666);

    connect(tcpClient2, SIGNAL(connected()), login, SLOT(show()));
    connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectError()));
    connect(tcpClient2, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectError()));
    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(tcpClient2, SIGNAL(readyRead()), tcpClient2, SLOT(readyRead()));
    connect(login, SIGNAL(sendLoginRequest(QByteArray)), tcpClient2, SLOT(sendRequest(QByteArray)));
    connect(tcpClient2, SIGNAL(getLoginResult(QDataStream&)), login, SLOT(getLoginResult(QDataStream&)));
    connect(login, SIGNAL(logInSuccess(QDataStream&)), this, SLOT(logInSuccess(QDataStream&)));
}

void MainView::logInSuccess(QDataStream &in) {
    QString face;
    int level, exp, money;
    in >> id >> face >> level >> exp >> money;
    person = new PersonDefin(username, QString(":face/image/face/%1-1.GIF").arg(face), level, exp, money);
    initMainView();
    sendUpdateRequest((1<<10) - 1);
}

void MainView::initMainView()
{
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

    connect(this, SIGNAL(getFriend(QDataStream&)), showfriendgroup, SLOT(getFriend(QDataStream&)));
    connect(this, SIGNAL(getSoils(QDataStream&)), soilgroup, SLOT(getSoils(QDataStream&)));
    show();
}

void MainView::createSoilGroup()
{
    soilgroup = new SoilGroup();
    connect(soilgroup,SIGNAL(sendSoil(Soil*)),this,SLOT(connectSoil(Soil*)));
    scene->addItem(soilgroup);
    soilgroup->setPos(475,430);
    soilgroup->createSoil();

    connect(soilgroup,SIGNAL(sendReclaRequest(int)),this,SLOT(sendReclaRequest(int)));
    connect(this, SIGNAL(reclaSuccess()),soilgroup,SLOT(reclaSuccess()));
}

void MainView::createToolGroup()
{
    toolgroup = new ToolGroup();
    connect(toolgroup,SIGNAL(sendTool(Tool*)),this,SLOT(connectTool(Tool*)));
    toolgroup->createTool();
    scene->addItem(toolgroup);
    toolgroup->setPos(220,600);
}

void MainView::createFriendGroup()
{
    showfriendgroup = new ShowFriendGroup();
    scene->addItem(showfriendgroup);
    showfriendgroup->setPos(1000, 325);
}

void MainView::createInforGroup()
{
    showinforgroup = new ShowInforGroup(person);
    scene->addItem(showinforgroup);
    showinforgroup->setPos(135,80);
}

void MainView::createSceneGroup()
{
    showscenegroup = new ShowSceneGroup(person->level);
    scene->addItem(showscenegroup);
    connect(showscenegroup,SIGNAL(sendStore(Store*)),this,SLOT(connectStore(Store*)));
    connect(showscenegroup,SIGNAL(sendShop(Shop*)),this,SLOT(connectShop(Shop*)));
    showscenegroup->createButton();
    showscenegroup->setPos(980,70);
}

void MainView::createPackGroup()
{
    packgroup = new PackGroup(person->level);
    packgroup->setParent(this);
    packgroup->setGeometry(150, 450, 700, 220);
    connect(this, SIGNAL(goodChange(Business,Good)), packgroup, SLOT(goodChange(Business,Good)));
    connect(this, SIGNAL(getGoods(QDataStream&,Business)), packgroup, SLOT(getGoods(QDataStream&,Business)));
    connect(packgroup, SIGNAL(sendBusinessRequest(Business,Good,int)), this, SLOT(sendBusinessRequest(Business,Good,int)));
    connect(this, SIGNAL(levelChange(int)), packgroup, SLOT(levelChange(int)));
    packgroup->hide();
}

void MainView::createChatWidget()
{
    chatWidget = new ChatWidget(username, password);
    chatWidget->setParent(this);
    chatWidget->setGeometry(150, 50, 700, 550);
    chatWidget->hide();
}

void MainView::connectSoil(Soil *soil)
{
    connect(this,SIGNAL(sendStatus(ToolType)),soil,SLOT(receiveStatus(ToolType)));

    connect(soil, SIGNAL(sendPlantRequest(int)), this, SLOT(sendPlantRequest(int)));
    connect(this, SIGNAL(plantSuccess(int,int,QDataStream&)), soil, SLOT(plantSuccess(int,int,QDataStream&)));

    connect(soil, SIGNAL(sendSpadRequest(int)), this, SLOT(sendSpadRequest(int)));
    connect(this, SIGNAL(spadSuccess(int)), soil, SLOT(spadSuccess(int)));

    connect(soil, SIGNAL(sendHarvestRequest(int)), this, SLOT(sendHarvestRequest(int)));
    connect(this, SIGNAL(harvestSuccess(int)), soil, SLOT(harvestSuccess(int)));

    connect(soil, SIGNAL(sendStatusChangeRequest(int,int,bool)), this, SLOT(sendStatusChangeRequest(int,int,bool)));
    connect(this, SIGNAL(statusChangeSuccess(int,int,bool,QDataStream&)), soil, SLOT(statusChangeSuccess(int,int,bool,QDataStream&)));

    connect(soil, SIGNAL(sendFertilizeRequest(int)), this, SLOT(sendFertilizeRequest(int)));
    connect(this, SIGNAL(fertilizeSuccess(int,int)), soil, SLOT(fertilizeSuccess(int,int)));
}

void MainView::connectTool(Tool *tool)
{
    connect(tool,SIGNAL(sendStatus(ToolType)),this,SLOT(receiveStatus(ToolType)));
}

void MainView::connectStore(Store *store)
{
    connect(this, SIGNAL(goodChange(Business,Good)), store, SLOT(goodChange(Business,Good)));
    connect(this, SIGNAL(levelChange(int)), store, SLOT(levelChange(int)));
    connect(this, SIGNAL(getGoods(QDataStream&,Business)), store, SLOT(getGoods(QDataStream&,Business)));
    connect(store, SIGNAL(sendBusinessRequest(Business,Good,int)), this, SLOT(sendBusinessRequest(Business,Good,int)));
}

void MainView::connectShop(Shop *shop)
{
    connect(this, SIGNAL(getGoods(QDataStream&,Business)), shop, SLOT(getGoods(QDataStream&,Business)));
    connect(shop, SIGNAL(sendBusinessRequest(Business,Good,int)), this, SLOT(sendBusinessRequest(Business,Good,int)));
    connect(this, SIGNAL(moneyChange(int)), shop, SLOT(moneyChange(int)));
    connect(this, SIGNAL(levelChange(int)), shop, SLOT(levelChange(int)));
}

void MainView::receiveStatus(ToolType newtype)
{
    if(newtype == Pack)
        packgroup->show();
    else if(newtype == Chat)
        chatWidget->show();
    else
    {
        toolType = newtype;
        showstatus->setPixmap(QPixmap(QString(":icon/image/icon/topbutton(")+QString::number(toolType)+QString(").png")));
        showstatus->show();
        emit sendStatus(toolType);
    }
}

void MainView::mouseMoveEvent(QMouseEvent *event)
{
    showstatus->setPos(event->x(),event->y()-30);
    QGraphicsView::mouseMoveEvent(event);
}

void MainView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        toolType = Empty;
        showstatus->hide();
        emit sendStatus(toolType);
    }
    QGraphicsView::mousePressEvent(event);
}

void MainView::connectError()
{
    QMessageBox::warning(this, "连接网络失败", tcpClient->errorString());
    exit(0);
    tcpClient->abort();
    tcpClient->connectToHost(QHostAddress("192.168.2.112"), 6666);
}

void MainView::sendUpdateRequest(int updateRequest)
{
    QByteArray outBlock;
    QDataStream out(&outBlock, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_5);
    out << qint64(0) << 3 << id << password << updateRequest;
    out.device()->seek(0);
    out << (qint64)outBlock.size();
    tcpClient->write(outBlock);
}

void MainView::getUpdateResult(QDataStream &in)
{
    int updateRequest;
    in >> updateRequest;
    if(updateRequest & 1)
        emit getSoils(in);
    if(updateRequest & 2)
        emit getFriend(in);
    if(updateRequest & 4)
        emit getGoods(in, Buy);
    if(updateRequest & 8)
        emit getGoods(in, Sell);
    if(updateRequest & 16)
        emit getGoods(in, Use);
}

void MainView::sendBusinessRequest(Business _business, Good _good, int _businessNum)
{
    toolType = Empty;
    emit sendStatus(Empty);
    business = _business;
    good = _good;
    businessNum = _businessNum;
    if(business == Use) {
        if(good.type == Seed)
        {
            toolType = Plant;
            showstatus->setPixmap(QPixmap(QString("%1/seed.png").arg(good.address)));
        }
        else if(good.type == Fertilize)
        {
            toolType = Ferti;
            showstatus->setPixmap(QPixmap(good.address));
        }
        packgroup->hide();
        showstatus->show();
        emit sendStatus(toolType);
    } else {
        QByteArray outBlock;
        QDataStream out(&outBlock, QIODevice::ReadWrite);
        out << qint64(0) << 5 << id << password << (int)_business << (int)_good.type << (int)_good.kind << _businessNum;
        out.device()->seek(0);
        out << (qint64)outBlock.size();
        tcpClient->write(outBlock);
    }
}

void MainView::getBusinessResult(QDataStream &in)
{
    int messageType;
    in >> messageType;
    if(messageType == 0)
        QMessageBox::warning(this, "交易失败", "交易数目不合法");
    else if(messageType == 1)
        QMessageBox::warning(this, "交易失败", "交易信息错误");
    else if(messageType == 2)
        QMessageBox::warning(this, "交易失败", "交易数目不合理");
    else if(messageType == 3)
    {
        if(business == Buy)
        {
            showinforgroup->moneyChange(person->money - good.buyPrice * businessNum);
            emit moneyChange(person->money);
        }
        else if(business == Sell)
        {
            if(good.type == Seed || good.type == Fertilize)
                showinforgroup->moneyChange(person->money + ((int)(good.buyPrice * 0.8)) * businessNum);
            else
                showinforgroup->moneyChange(person->money + good.sellPrice * businessNum);
            emit moneyChange(person->money);
        }
        if(business == Buy)
            good.num = businessNum;
        else if(business == Sell)
            good.num -= businessNum;
        else if(business == Use)
            good.num--;
        emit goodChange(business, good);
    }
}

void MainView::sendPlantRequest(int _number)
{
    if(good.type != Seed || _number > 18 || _number < 1)
    {
        QMessageBox::warning(this, "数据有误", "请尝试重启更新数据以解决问题");
        return;
    }
    QByteArray outBlock;
    QDataStream out(&outBlock, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_5);
    out << qint64(0) << 4 << id << password << _number << good.kind;
    out.device()->seek(0);
    out << (qint64)outBlock.size();
    tcpClient->write(outBlock);
}

void MainView::getPlantResult(QDataStream &in)
{
    int messageType;
    in >> messageType;
    if(messageType == 0)
        QMessageBox::warning(this, "种植失败", "种子不足，请尝试重启更新数据");
    else if(messageType == 1)
        QMessageBox::warning(this, "种植失败", "土地信息有误，请尝试重启");
    else if(messageType == 2)
    {
        int number;
        in >> number;
        emit plantSuccess(number, good.kind, in);
        good.num--;
        emit goodChange(Use, good);
        if(!good.num)
        {
            showstatus->hide();
            toolType = Empty;
            emit sendStatus(toolType);
        }
    }
}

void MainView::sendSpadRequest(int _number)
{
    if(_number > 18 || _number < 1)
    {
        QMessageBox::warning(this, "数据有误", "请尝试重启更新数据以解决问题");
        return;
    }
    QByteArray outBlock;
    QDataStream out(&outBlock, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_5);
    out << qint64(0) << 6 << id << password << _number;
    out.device()->seek(0);
    out << (qint64)outBlock.size();
    tcpClient->write(outBlock);
}

void MainView::getSpadResult(QDataStream &in)
{
    int messageType;
    in >> messageType;
    if(messageType == 0)
        QMessageBox::warning(this, "铲除失败", "土地信息有误，请尝试重启");
    else if(messageType == 1)
    {
        int number;
        in >> number;
        emit spadSuccess(number);
    }
}

void MainView::sendHarvestRequest(int _number)
{
    if(_number > 18 || _number < 1)
    {
        QMessageBox::warning(this, "数据有误", "请尝试重启更新数据以解决问题");
        return;
    }
    QByteArray outBlock;
    QDataStream out(&outBlock, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_5);
    out << qint64(0) << 7 << id << password << _number;
    out.device()->seek(0);
    out << (qint64)outBlock.size();
    tcpClient->write(outBlock);
}

void MainView::getHarvestResult(QDataStream &in)
{
    int messageType;
    in >> messageType;
    if(messageType == 0)
        QMessageBox::warning(this, "收取失败", "土地信息有误，请尝试重启");
    else if(messageType == 1)
        QMessageBox::warning(this, "收取失败", "当前植物未成熟，请尝试重启");
    else if(messageType == 2)
    {
        int number;
        in >> number;
        emit harvestSuccess(number);
        int kind, num, exp;
        in >> kind >> num >> exp;
        if(showinforgroup->expAdd(exp))
            emit levelChange(person->level);
        emit goodChange(Buy, createGood(Fruit, kind, num));
    }
}

void MainView::sendStatusChangeRequest(int _status, int _number, bool _auto)
{
    if(_number > 18 || _number < 1)
    {
        QMessageBox::warning(this, "数据有误", "请尝试重启更新数据以解决问题");
        return;
    }
    QByteArray outBlock;
    QDataStream out(&outBlock, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_5);
    out << qint64(0) << 8 << id << password << _number << _status << _auto;
    out.device()->seek(0);
    out << (qint64)outBlock.size();
    tcpClient->write(outBlock);
}

void MainView::getStatusChangeResult(QDataStream &in)
{
    int messageType;
    in >> messageType;
    if(messageType == 0)
        QMessageBox::warning(this, "数据异常", "建议立即重启，以免造成损失");
    else if(messageType == 1)
    {
        int number, _status;
        bool isAuto;
        in >> number >> _status >> isAuto;
        emit statusChangeSuccess(number, _status, isAuto, in);
    }
}

void MainView::sendReclaRequest(int number)
{
    if(number > 17 || number < 0)
    {
        QMessageBox::warning(this, "数据有误", "请尝试重启更新数据以解决问题");
        return;
    }
    if(QMessageBox::question(NULL,"开垦",QString("开垦该土地需要金币%1,等级%2,是否确定开垦?").arg(number).arg(number/3+1),
                             QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes)
    {
        if(person->money < number || person->level < number/3+1)
        {
            QMessageBox::warning(this, "开垦失败", "条件未满足!");
            return;
        }
        QByteArray outBlock;
        QDataStream out(&outBlock, QIODevice::ReadWrite);
        out.setVersion(QDataStream::Qt_5_5);
        out << qint64(0) << 9 << id << password << number;
        out.device()->seek(0);
        out << (qint64)outBlock.size();
        tcpClient->write(outBlock);
    }
}

void MainView::getReclaResult(QDataStream &in)
{
    int messageType;
    in >> messageType;
    if(messageType == 0)
        QMessageBox::warning(this, "开垦失败", "当前土地不允许被开垦");
    else if(messageType == 1)
        QMessageBox::warning(this, "开垦失败", "当前条件不满足");
    else if(messageType == 2)
    {
        QMessageBox::warning(this, "开垦成功", "当前土地成功开垦，可以进行种植了");
        int money;
        in >> money;
        showinforgroup->moneyChange(money);
        emit moneyChange(money);
        emit reclaSuccess();
    }
}

void MainView::sendFertilizeRequest(int number)
{
    if(number > 18 || number < 1)
    {
        QMessageBox::warning(this, "数据有误", "请尝试重启更新数据以解决问题");
        return;
    }
    QByteArray outBlock;
    QDataStream out(&outBlock, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_5);
    out << qint64(0) << 10 << id << password << number << good.kind;
    out.device()->seek(0);
    out << (qint64)outBlock.size();
    tcpClient->write(outBlock);
}

void MainView::getFertilizeResult(QDataStream &in)
{
    int messageType;
    in >> messageType;
    if(messageType == 0)
        QMessageBox::warning(this, "施肥失败", "化肥不足，请尝试重启更新数据");
    else if(messageType == 1)
        QMessageBox::warning(this, "施肥失败", "土地信息有误，请尝试重启");
    else if(messageType == 2)
    {
        int number, reduTime;
        in >> number >> reduTime;
        emit fertilizeSuccess(number, reduTime);
        good.num--;
        emit goodChange(Use, good);
        if(!good.num)
        {
            showstatus->hide();
            toolType = Empty;
            emit sendStatus(toolType);
        }
    }
}

void MainView::readyRead()
{
    if(!tcpClient->bytesAvailable())
        return;
    inBlock.append(tcpClient->readAll());
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
        case 3:
            getUpdateResult(in);
            break;
        case 4:
            getPlantResult(in);
            break;
        case 5:
            getBusinessResult(in);
            break;
        case 6:
            getSpadResult(in);
            break;
        case 7:
            getHarvestResult(in);
            break;
        case 8:
            getStatusChangeResult(in);
            break;
        case 9:
            getReclaResult(in);
            break;
        case 10:
            getFertilizeResult(in);
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

MainView::~MainView()
{
}
