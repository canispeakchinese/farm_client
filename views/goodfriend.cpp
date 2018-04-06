#include "goodfriend.h"
#include "dal/baseview/buttonitem.h"
#include "showinforgroup.h"
#include <QPainter>
#include <QEvent>
#include <QMessageBox>
#include <QDebug>

GoodFriend::GoodFriend(int number, PersonDefin person) :
    number(number), person(person)
{
}

QRectF GoodFriend::boundingRect() const//我猜是用来确定刷新区域的，但是不确定
{
    return QRectF(-85, -20, 170, 40);
}

void GoodFriend::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawText(QRectF(-80, -8, 30, 16), QString::number(number));
    painter->drawPixmap(-50, -18, 36, 36, QPixmap(person.faceaddress));
    painter->drawText(QRectF(-10, -4, 50, 24), person.username);
}

QPainterPath GoodFriend::shape() const//这个函数返回值确定的是这个item的作用范围，比如点击事件,碰撞检测
{
    QPainterPath path;
    path.addRect(-85, -20, 170, 40);
    return path;
}

bool GoodFriend::sceneEvent(QEvent *event)
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
    return QGraphicsObject::sceneEvent(event);
}

void GoodFriend::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsObject::mousePressEvent(event);
}

GoodFriend::~GoodFriend()
{
}

FriendGroup::FriendGroup(QVector<PersonDefin> persons) : persons(persons)
{
    currpage = 0;
    pagenum = 0;
    left = new ButtonItem(-20, -10, 40, 20, QPixmap(QString(":guiRes/image/guiRes/left.png")));
    right = new ButtonItem(-20, -10, 40, 20, QPixmap(QString(":guiRes/image/guiRes/right.png")));
    addToGroup(left);
    addToGroup(right);
    left->setPos(25, 235);
    right->setPos(155, 235);
    connect(left,SIGNAL(clicked()),this,SLOT(front()));
    connect(right,SIGNAL(clicked()),this,SLOT(next()));
}

QRectF FriendGroup::boundingRect() const//我猜是用来确定刷新区域的，但是不确定
{
    return QRectF(0, -250, 180, 500);
}

void FriendGroup::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawPixmap(0, -250, 180, 500, QPixmap(QString(":guiRes/image/guiRes/muban.jpg")));
    painter->drawText(QRect(70, 225, 50, 20),QString("%1/%2").arg(currpage).arg(pagenum));
    painter->setBrush(QColor(255, 235, 205));
    painter->drawRect(3, -180, 174, 400);
}

QPainterPath FriendGroup::shape() const//这个函数返回值确定的是这个item的作用范围，比如点击事件,碰撞检测
{
    QPainterPath path;
    path.addRect(0,-250,180,500);
    return path;
}

void FriendGroup::addshowfriend()
{
    if(pagenum == 0)
        return;
    if(currpage <= 0 || currpage > pagenum)
        currpage = 1;
    for(int i=(currpage-1)*10; i<currpage*10 && i<persons.size(); i++)
    {
        GoodFriend *goodFriend = new GoodFriend(i+1, persons[i]);
        addToGroup(goodFriend);
        goodFriend->setPos(90, -160+40*(i-(currpage-1)*10));
        items << goodFriend;
    }
}

void FriendGroup::removeshowfriend()
{
    foreach(GoodFriend * item, items)
    {
        removeFromGroup(item);
        item->~GoodFriend();
    }
    items.clear();
}

void FriendGroup::updateFriend(QVector<PersonDefin> _persons)
{
    persons = _persons;
    pagenum = persons.size()/10;
    if(persons.size()%10)
        pagenum++;
    removeshowfriend();
    addshowfriend();
}

void FriendGroup::front()
{
    if(currpage <= 1)
        return;
    removeshowfriend();
    currpage--;
    addshowfriend();
}

void FriendGroup::next()
{
    if(currpage == pagenum)
        return;
    removeshowfriend();
    currpage++;
    addshowfriend();
}

FriendGroup::~FriendGroup()
{

}

