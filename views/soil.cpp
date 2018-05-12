#include "soil.h"
#include "views/showinforgroup.h"
#include "dal/baseview/buttonitem.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QDebug>

const int duration = 3600;

Soil::Soil(int num) : num(num), mature(new QTimer()), water(new QTimer()),
    pyre(new QTimer()),weed(new QTimer()), timer(new QTimer()) {
    memset(need,false,sizeof(need));
    isRecla = false;
    connect(timer, SIGNAL(timeout()), this, SLOT(update2()));
    connect(mature, SIGNAL(timeout()), this, SLOT(matureChange()));
    connect(water, SIGNAL(timeout()), this, SLOT(waterChange()));
    connect(pyre, SIGNAL(timeout()), this, SLOT(pyreChange()));
    connect(weed, SIGNAL(timeout()), this, SLOT(weedChange()));
}

void Soil::updateInfor(QDataStream &in) {
    in >> level >> isRecla;
    qDebug() << "Soil " << num << " level: " << level << ", recla info: " << isRecla;
    if(isRecla) {
        in >> kind;
        if(!kind) {
            need[Plant] = true;
        } else {
            in >> isHarvest;
            if(!isHarvest) {
                in >> isMature;
                if(isMature) {
                    in >> allSta >> yield;
                } else {
                    in >> growTime >> allSta >> yield;
                    int time;
                    in >> time;
                    mature->start(time * 1000);
                    in >> need[Water] >> time;
                    water->start(time * 1000 + 3000);
                    in >> need[Pyre] >> time;
                    pyre->start(time * 1000 + 3000);
                    in >> need[Weed] >> time;
                    weed->start(time * 1000 + 3000);
                    timer->start(1000);
                }
            }
        }
    }
}

void Soil::update2() {
    update();
}

QRectF Soil::boundingRect() const {
    return QRectF(-90,-40,180,80);
}

void Soil::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if(isRecla) {
        if(!need[Water]) {
            painter->drawPixmap(-90, -40, 180, 80, QPixmap(QString(":soil/image/soil/%1.png").arg(level)));
        } else {
            painter->drawPixmap(-90, -40, 180, 80, QPixmap(QString(":soil/image/soil/5.png")));
        }
        if(!need[Plant])
        {
            if(isHarvest) {
                painter->drawPixmap(-90, -40, 180, 60, QPixmap(QString("%1/cron_end.png").arg(goodDefin[Seed][kind].address)));
            } else if(isMature) {
                painter->drawPixmap(-90, -40, 180, 60, QPixmap(QString("%1/%2.png").arg(goodDefin[Seed][kind].address).arg(allSta)));
            } else {
                if(need[Pyre])
                    painter->drawPixmap(20, -10, 40, 20, QPixmap(":/worm/image/worm/1.png"));
                currSta = allSta - (mature->remainingTime()/1000)/(growTime * 3600);
                if((mature->remainingTime()/1000)%(growTime * 3600))
                    currSta--;
                if(currSta == 0)
                    painter->drawPixmap(-90, -40, 180, 60, QPixmap(QString("%1/cron_start.png").arg(goodDefin[Seed][kind].address)));
                else
                    painter->drawPixmap(-90, -40, 180, 60, QPixmap(QString("%1/%2.png").arg(goodDefin[Seed][kind].address).arg(currSta)));
                painter->drawText(QRect(-90, -40, 180, 20), Qt::AlignCenter, QTime(0, 0, 0).addSecs((mature->remainingTime()/1000)%(growTime * 3600)).toString("hh:mm:ss"));
            }
        }
    }
    else {
        painter->drawPixmap(-90, -40, 180, 80, QPixmap(QString(":soil/image/soil/0.png")));
    }
}

QPainterPath Soil::shape() const {
    QPainterPath path;
    QPolygon temp;
    temp << QPoint(-90,0) << QPoint(0,-40)  <<QPoint(90,0) << QPoint(0,40);
    path.addPolygon(temp);
    return path;
}

bool Soil::sceneEvent(QEvent *event) {
    if(event->type() == QEvent::GraphicsSceneMousePress) {
        mousePressEvent((QGraphicsSceneMouseEvent *)event);
    } else if(event->type() == QEvent::GraphicsSceneMouseRelease) {
        mouseReleaseEvent((QGraphicsSceneMouseEvent *)event);
    } else if(event->type() == QEvent::GraphicsSceneMouseMove) {
        mouseMoveEvent((QGraphicsSceneMouseEvent *)event);
    } else if(event->type() == QEvent::GraphicsSceneMouseDoubleClick) {
        mouseDoubleClickEvent((QGraphicsSceneMouseEvent *)event);
    }
    return QGraphicsItem::sceneEvent(event);
}

void Soil::matureChange() {
    mature->stop();
    water->stop();
    pyre->stop();
    weed->stop();
    isMature = true;
    memset(need, false, sizeof(need));
}

void Soil::waterChange() {
    if(!need[Water]) {
        need[Water] = true;
        water->start(duration * 1000);
        return;
    }
    water->stop();
    emit sendStatusChangeRequest(createStatusChangeRequest((int)Water));
}

void Soil::pyreChange() {
    if(!need[Pyre]) {
        need[Pyre] = true;
        pyre->start(duration * 1000);
        return;
    }
    pyre->stop();
    emit sendStatusChangeRequest(createStatusChangeRequest((int)Pyre));
}

void Soil::weedChange() {
    if(!need[Weed]) {
        need[Weed] = true;
        weed->start(duration * 1000);
        return;
    }
    weed->stop();
    emit sendStatusChangeRequest(createStatusChangeRequest((int)Weed));
}

void Soil::plantSuccess(int _kind, QDataStream &in) {
    memset(need, false, sizeof(need));
    isHarvest = false;
    isMature = false;
    kind = _kind;
    in >> growTime >> allSta >> yield;
    mature->start(growTime * allSta * 3600 * 1000);
    int time1, time2, time3;
    in >> time1 >> time2 >> time3;
    water->start(time1 * 1000);
    pyre->start(time2 * 1000);
    weed->start(time3 * 1000);
    timer->start(1000);
}

void Soil::spadSuccess() {
    memset(need, false, sizeof(need));
    need[Plant] = true;
    mature->stop();
    water->stop();
    pyre->stop();
    weed->stop();
    timer->stop();
    update();
}

void Soil::harvestSuccess() {
    isHarvest = true;
    memset(need, false, sizeof(need));
    need[Spad] = true;
    timer->stop();
    isHarvest = true;
    update();
}

void Soil::statusChangeSuccess(int changeStatus, bool _auto, QDataStream &in) {
    if(_auto) {
        if(changeStatus == Pyre)
            yield--;
    } else {
        if(changeStatus == Water)
            yield++;
    }
    int time;
    in >> need[changeStatus] >> time;
    if(changeStatus == Water)
        water->start(time * 1000 + 3000);
    else if(changeStatus == Pyre)
        pyre->start(time * 1000 + 3000);
    else if(changeStatus == Weed)
        weed->start(time * 1000 + 3000);
}

void Soil::reclaSuccess() {
    isRecla = true;
    need[Plant] = true;
    update();
}

void Soil::fertilizeSuccess(int reduTime) {
    if(mature->remainingTime() < reduTime * 1000)
        matureChange();
    else
        mature->start(mature->remainingTime() - reduTime * 1000);
}

QByteArray Soil::createStatusChangeRequest(int status, bool _auto) {
    QByteArray outBlock;
    QDataStream out(&outBlock, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_5);
    out << qint64(0) << 8 << num << status << _auto;
    out.device()->seek(0);
    out << (qint64)outBlock.size();
    return outBlock;
}

void Soil::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    ToolType status = MainView::getStatus();
    if(status == Plant && need[Plant]) {
        QByteArray outBlock;
        QDataStream out(&outBlock, QIODevice::ReadWrite);
        Good good = MainView::getGood();
        qDebug() << "Soil " << num << " Plant " << good.name;
        out.setVersion(QDataStream::Qt_5_5);
        out << qint64(0) << 4 << num << good.kind;
        out.device()->seek(0);
        out << (qint64)outBlock.size();
        emit sendPlantRequest(outBlock);
    } else if(status == Spad && !need[Plant] && isRecla) {
        if(isHarvest || QMessageBox::question(NULL, "警告", "该土地还未收获，是否确定铲除", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
            qDebug() << "Soil " << num << " Start Spad";
            QByteArray outBlock;
            QDataStream out(&outBlock, QIODevice::ReadWrite);
            out.setVersion(QDataStream::Qt_5_5);
            out << qint64(0) << 6 << num;
            out.device()->seek(0);
            out << (qint64)outBlock.size();
            emit sendSpadRequest(outBlock);
        }
    } else if(status == Harv && !need[Plant] && isRecla && isMature && !isHarvest) {
        QByteArray outBlock;
        QDataStream out(&outBlock, QIODevice::ReadWrite);
        out.setVersion(QDataStream::Qt_5_5);
        out << qint64(0) << 7 << num;
        out.device()->seek(0);
        out << (qint64)outBlock.size();
        emit sendHarvestRequest(outBlock);
    } else if((status == Water || status == Pyre || status == Weed) && need[status]) {
        emit sendStatusChangeRequest(createStatusChangeRequest(status, false));
    } else if(status == Ferti && isRecla && !need[Plant] && !isHarvest && !isMature) {
        emit getFertilize(num);
    }
    QGraphicsItem::mousePressEvent(event);
}

Soil::~Soil() {
}

SoilGroup::SoilGroup(MainView *parent): parent(parent) {
    source = SoilGroupSource;
    reclaButton = new ButtonItem(-40, -20, 80, 20, "开垦");
    reclaButton->setZValue(1);
    addToGroup(reclaButton);
    connect(reclaButton, SIGNAL(clicked()), this, SLOT(tryRecla()));
}

void SoilGroup::createSoil() {
    for(int i=0;i<6;i++) {
        int x = 90*i-225;
        int y = 40*i-100;
        for(int j=0;j<3;j++) {
            soil[i*3+j] = new Soil(i*3+j+1);
            connect(soil[i*3+j], SIGNAL(getFertilize(int)), this, SLOT(fertilizeEvent(int)));
            connect(soil[i*3+j], SIGNAL(sendPlantRequest(QByteArray)), this, SLOT(getRequestFromSoil(QByteArray)));
            connect(soil[i*3+j], SIGNAL(sendSpadRequest(QByteArray)), this, SLOT(getRequestFromSoil(QByteArray)));
            connect(soil[i*3+j], SIGNAL(sendHarvestRequest(QByteArray)), this, SLOT(getRequestFromSoil(QByteArray)));
            connect(soil[i*3+j], SIGNAL(sendStatusChangeRequest(QByteArray)), this, SLOT(getRequestFromSoil(QByteArray)));
            addToGroup(soil[i*3+j]);
            soil[i*3+j]->setPos(QPoint(90-90*j+x,40*j-40+y));
        }
    }
}

QRectF SoilGroup::boundingRect() const {
    return QRectF(-405,-180,810,360);
}

void SoilGroup::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    Q_UNUSED(painter);
}

QPainterPath SoilGroup::shape() const//这个函数返回值确定的是这个item的作用范围，比如点击事件,碰撞检测
{
    QPainterPath path;
    QPolygon temp;
    temp << QPoint(-405, -60) << QPoint(-135, -180)  <<QPoint(405, 60) << QPoint(135 , 180);
    path.addPolygon(temp);
    return path;
}

void SoilGroup::getSoils(QDataStream &in)
{
    in >> reclaNum;
    if(reclaNum) {
        reclaButton->setPos(soil[reclaNum-1]->x(), soil[reclaNum-1]->y());
    } else {
        reclaButton->hide();
    }
    for(int i=0; i<18; i++) {
        soil[i]->updateInfor(in);
    }
    MainView::updateMutex.unlock();
}

void SoilGroup::reclaSuccess()
{
    soil[reclaNum-1]->reclaSuccess();
    reclaNum = (reclaNum + 1) % 18;
    if(reclaNum)
        reclaButton->setPos(soil[reclaNum-1]->x(), soil[reclaNum-1]->y());
    else
        reclaButton->hide();
}

void SoilGroup::tryRecla()
{
    int number = reclaNum-1;
    if(QMessageBox::question(NULL,"开垦",QString("开垦该土地需要金币%1,等级%2,是否确定开垦?").arg(number).arg(number/3+1), QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes) {
        UserInfo person = MainView::getUserInfo();
        if(person.money < number || person.level < number/3+1) {
            QMessageBox::warning(parent, "开垦失败", "条件未满足!");
            return;
        }
        QByteArray outBlock;
        QDataStream out(&outBlock, QIODevice::ReadWrite);
        out.setVersion(QDataStream::Qt_5_5);
        out << qint64(0) << 9 << number;
        out.device()->seek(0);
        out << (qint64)outBlock.size();
        emit sendSoilRequestToServer(outBlock);
    }
}

void SoilGroup::getPlantResult(QDataStream& in) {
    int messageType;
    in >> messageType;
    if(messageType == 0)
        QMessageBox::warning(parent, "种植失败", "种子不足，请尝试重启更新数据");
    else if(messageType == 1)
        QMessageBox::warning(parent, "种植失败", "土地信息有误，请尝试重启");
    else if(messageType == 2) {
        int number;
        in >> number;
        Good good = MainView::getGood();
        qDebug() << "Soil " << number << " Plant " << good.name << " Success";
        soil[number-1]->plantSuccess(good.kind, in);
        good.num--;
        MainView::setGood(good);
        emit goodChange(SoilGroupSource, GoodChange::Lose);
        if(!good.num) {
            qDebug() << "SoilGroup Plant All Seed, Status Changed: " << MainView::getStatus() << " -> " << Empty;
            MainView::setStatus(Empty);
            emit statusChange(source);
        }
    }
}

void SoilGroup::getSpadResult(QDataStream &in)
{
    int messageType;
    in >> messageType;
    if(messageType == 0)
        QMessageBox::warning(parent, "铲除失败", "土地信息有误，请尝试重启");
    else if(messageType == 1) {
        int number;
        in >> number;
        qDebug() << "Soil " << number << " Spad Success";
        soil[number-1]->spadSuccess();
    }
}

void SoilGroup::getHarvestResult(QDataStream &in)
{
    int messageType;
    in >> messageType;
    if(messageType == 0) {
        QMessageBox::warning(parent, "收取失败", "土地信息有误，请尝试重启");
    } else if(messageType == 1) {
        QMessageBox::warning(parent, "收取失败", "当前植物未成熟，请尝试重启");
    } else if(messageType == 2) {
        int number;
        in >> number;
        soil[number-1]->harvestSuccess();
        int kind, num, exp;
        in >> kind >> num >> exp;
        qDebug() << "Harvest Success, Fruit Kind is " << kind << ", Num is " << num << ", Get Exp " << exp;
        UserInfo userInfo = MainView::getUserInfo();
        userInfo.exp += exp;
        MainView::setUserInfo(userInfo);
        emit expChange(SoilGroupSource);
        MainView::setGood(createGood(Fruit, kind, num));
        emit goodChange(SoilGroupSource, GoodChange::Get);
    }
}

void SoilGroup::getStatusChangeResult(QDataStream &in)
{
    int messageType;
    in >> messageType;
    if(messageType == 0)
        QMessageBox::warning(parent, "数据异常", "建议立即重启，以免造成损失");
    else if(messageType == 1) {
        int number, _status;
        bool isAuto;
        in >> number >> _status >> isAuto;
        soil[number-1]->statusChangeSuccess(_status, isAuto, in);
    }
}

void SoilGroup::getReclaResult(QDataStream &in)
{
    int messageType;
    in >> messageType;
    if(messageType == 0)
        QMessageBox::warning(parent, "开垦失败", "当前土地不允许被开垦");
    else if(messageType == 1)
        QMessageBox::warning(parent, "开垦失败", "当前条件不满足");
    else if(messageType == 2) {
        QMessageBox::warning(parent, "开垦成功", "当前土地成功开垦，可以进行种植了");
        int money;
        in >> money;
        UserInfo userInfo = MainView::getUserInfo();
        userInfo.money = money;
        MainView::setUserInfo(userInfo);
        qDebug() << "Recla New Soil Success, Now Money is " << money;
        emit moneyChange(SoilGroupSource);
        emit reclaSuccess();
    }
}

void SoilGroup::getFertilizeResult(QDataStream &in)
{
    int messageType;
    in >> messageType;
    if(messageType == 0)
        QMessageBox::warning(parent, "施肥失败", "化肥不足，请尝试重启更新数据");
    else if(messageType == 1)
        QMessageBox::warning(parent, "施肥失败", "土地信息有误，请尝试重启");
    else if(messageType == 2) {
        int number, reduTime;
        in >> number >> reduTime;
        soil[number-1]->fertilizeSuccess(reduTime);
        Good good = MainView::getGood();
        good.num--;
        qDebug() << "Soil " << number << " Fertilize Success, Redue Time " << reduTime;
        MainView::setGood(good);
        emit goodChange(MainViewSource, GoodChange::Lose);
        if(!good.num) {
            qDebug() << "SoilGroup Can Not Fertilize, Status Changed: " << MainView::getStatus() << " -> " << Empty;
            MainView::setStatus(Empty);
            emit statusChange(source);
        }
    }
}

void SoilGroup::fertilizeEvent(int num) {
    QByteArray outBlock;
    QDataStream out(&outBlock, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_5);
    Good good = MainView::getGood();
    qDebug() << "SoilGroup Fertilize " << good.name;
    out << qint64(0) << 10 << num << good.kind;
    out.device()->seek(0);
    out << (qint64)outBlock.size();
    emit sendSoilRequestToServer(outBlock);
}


SoilGroup::~SoilGroup()
{

}
