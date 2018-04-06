#include "soil.h"
#include "dal/baseview/buttonitem.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QDebug>

const int duration = 3600;

Soil::Soil(int num) : num(num), mature(new QTimer()), water(new QTimer()),
    pyre(new QTimer()),weed(new QTimer()), timer(new QTimer())
{   
    status = Empty;
    memset(need,false,sizeof(need));
    isRecla = false;
    connect(timer, SIGNAL(timeout()), this, SLOT(update2()));
    connect(mature, SIGNAL(timeout()), this, SLOT(matureChange()));
    connect(water, SIGNAL(timeout()), this, SLOT(waterChange()));
    connect(pyre, SIGNAL(timeout()), this, SLOT(pyreChange()));
    connect(weed, SIGNAL(timeout()), this, SLOT(weedChange()));
}

void Soil::updateInfor(QDataStream &in)
{
    in >> level >> isRecla;
    if(isRecla)
    {
        in >> kind;
        if(!kind)
            need[Plant] = true;
        else
        {
            in >> isHarvest;
            if(!isHarvest)
            {
                in >> isMature;
                if(isMature)
                    in >> allSta >> yield;
                else
                {
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

void Soil::update2()
{
    update();
}

QRectF Soil::boundingRect() const//我猜是用来确定刷新区域的，但是不确定
{
    return QRectF(-90,-40,180,80);
}

void Soil::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if(isRecla)
    {
        if(!need[Water])
            painter->drawPixmap(-90, -40, 180, 80, QPixmap(QString(":soil/image/soil/%1.png").arg(level)));
        else
            painter->drawPixmap(-90, -40, 180, 80, QPixmap(QString(":soil/image/soil/5.png")));
        if(!need[Plant])
        {
            if(isHarvest)
                painter->drawPixmap(-90, -40, 180, 60, QPixmap(QString("%1/cron_end.png").arg(goodDefin[Seed][kind].address)));
            else if(isMature)
                painter->drawPixmap(-90, -40, 180, 60, QPixmap(QString("%1/%2.png").arg(goodDefin[Seed][kind].address).arg(allSta)));
            else
            {
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
    else
        painter->drawPixmap(-90, -40, 180, 80, QPixmap(QString(":soil/image/soil/0.png")));
}

QPainterPath Soil::shape() const//这个函数返回值确定的是这个item的作用范围，比如点击事件,碰撞检测
{
    QPainterPath path;
    QPolygon temp;
    temp << QPoint(-90,0) << QPoint(0,-40)  <<QPoint(90,0) << QPoint(0,40);
    path.addPolygon(temp);
    return path;
}

bool Soil::sceneEvent(QEvent *event)
{
    if(event->type() == QEvent::GraphicsSceneMousePress)
    {
        mousePressEvent((QGraphicsSceneMouseEvent *)event);
    }
    else if(event->type() == QEvent::GraphicsSceneMouseRelease)
    {
        mouseReleaseEvent((QGraphicsSceneMouseEvent *)event);
    }
    else if(event->type() == QEvent::GraphicsSceneMouseMove)
    {
        mouseMoveEvent((QGraphicsSceneMouseEvent *)event);
    }
    else if(event->type() == QEvent::GraphicsSceneMouseDoubleClick)
    {
        mouseDoubleClickEvent((QGraphicsSceneMouseEvent *)event);
    }
    return QGraphicsItem::sceneEvent(event);
}

void Soil::matureChange()
{
    mature->stop();
    water->stop();
    pyre->stop();
    weed->stop();
    isMature = true;
    memset(need, false, sizeof(need));
}

void Soil::waterChange()
{
    if(!need[Water])
    {
        need[Water] = true;
        water->start(duration * 1000);
        return;
    }
    water->stop();
    emit sendStatusChangeRequest((int)Water, num);
}

void Soil::pyreChange()
{
    if(!need[Pyre])
    {
        need[Pyre] = true;
        pyre->start(duration * 1000);
        return;
    }
    pyre->stop();
    emit sendStatusChangeRequest((int)Pyre, num);
}

void Soil::weedChange()
{
    if(!need[Weed])
    {
        need[Weed] = true;
        weed->start(duration * 1000);
        return;
    }
    weed->stop();
    emit sendStatusChangeRequest((int)Weed, num);
}

void Soil::plantSuccess(int _num, int _kind, QDataStream &in)
{
    if(num != _num)
        return;
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

void Soil::spadSuccess(int _num)
{
    if(num != _num)
        return;
    memset(need, false, sizeof(need));
    need[Plant] = true;
    mature->stop();
    water->stop();
    pyre->stop();
    weed->stop();
    timer->stop();
    update();
}

void Soil::harvestSuccess(int _num)
{
    if(num != _num)
        return;
    isHarvest = true;
    memset(need, false, sizeof(need));
    need[Spad] = true;
    timer->stop();
    isHarvest = true;
    update();
}

void Soil::statusChangeSuccess(int _num, int changeStatus, bool _auto, QDataStream &in)
{
    if(num != _num)
        return;
    if(_auto)
    {
        if(changeStatus == Pyre)
            yield--;
    }
    else
    {
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

void Soil::reclaSuccess()
{
    isRecla = true;
    need[Plant] = true;
    update();
}

void Soil::fertilizeSuccess(int _num, int reduTime)
{
    if(num != _num)
        return;
    if(mature->remainingTime() < reduTime * 1000)
        matureChange();
    else
        mature->start(mature->remainingTime() - reduTime * 1000);
}

void Soil::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(status == Plant && need[Plant])
        emit sendPlantRequest(num);
    else if(status == Spad && !need[Plant] && isRecla)
    {
        if(isHarvest)
            emit sendSpadRequest(num);
        else if(QMessageBox::question(NULL,"警告","该土地还未收获,是否确定铲除",QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes)
            emit sendSpadRequest(num);
    }
    else if(status == Harv && !need[Plant] && isRecla && isMature && !isHarvest)
        emit sendHarvestRequest(num);
    else if((status == Water || status == Pyre || status == Weed) && need[status])
        emit sendStatusChangeRequest((int)status, num, false);
    else if(status == Ferti && isRecla && !need[Plant] && !isHarvest && !isMature)
        emit sendFertilizeRequest(num);
    QGraphicsItem::mousePressEvent(event);
}

void Soil::receiveStatus(ToolType type)
{
    status = type;
}

Soil::~Soil()
{
}

SoilGroup::SoilGroup()
{
    reclaButton = new ButtonItem(-40, -20, 80, 20, "开垦");
    reclaButton->setZValue(1);
    addToGroup(reclaButton);
    connect(reclaButton, SIGNAL(clicked()), this, SLOT(tryRecla()));
}

void SoilGroup::createSoil()
{
    for(int i=0;i<6;i++)
    {
        int x = 90*i-225;
        int y = 40*i-100;
        for(int j=0;j<3;j++)
        {
            soil[i*3+j] = new Soil(i*3+j+1);
            addToGroup(soil[i*3+j]);
            emit sendSoil(soil[i*3+j]);
            soil[i*3+j]->setPos(QPoint(90-90*j+x,40*j-40+y));
        }
    }
}

QRectF SoilGroup::boundingRect() const//我猜是用来确定刷新区域的，但是不确定
{
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
    if(reclaNum)
        reclaButton->setPos(soil[reclaNum-1]->x(), soil[reclaNum-1]->y());
    else
        reclaButton->hide();
    for(int i=0; i<18; i++)
        soil[i]->updateInfor(in);
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
    sendReclaRequest(reclaNum-1);
}

SoilGroup::~SoilGroup()
{

}
