#include "showfriendgroup.h"
#include "goodfriend.h"
#include "showinforgroup.h"
#include "dal/baseview/buttonitem.h"
#include <QPainter>
#include <QMessageBox>
#include <QEvent>

ShowFriend::ShowFriend()
{
    is_show = false;
}

QRectF ShowFriend::boundingRect() const
{
    return QRectF(-18, -40, 18, 80);
}

void ShowFriend::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setBrush(Qt::yellow);
    painter->setPen(Qt::yellow);
    painter->drawRect(-20, -43, 20, 86);
    painter->setPen(Qt::black);
    painter->drawText(QRectF(-18, -40, 18, 20) ,"我");
    painter->drawText(QRectF(-18, -20, 18, 20), "的");
    painter->drawText(QRectF(-18, 0, 18, 20), "好");
    painter->drawText(QRectF(-18, 20, 18, 20), "友");
}

QPainterPath ShowFriend::shape() const
{
    QPainterPath path;
    path.addRect(-18, -40, 18, 80);
    return path;
}

bool ShowFriend::sceneEvent(QEvent *event)
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

void ShowFriend::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(!is_show)
    {
        is_show = true;
        emit showfriend();
    }
    else
    {
        is_show = false;
        emit hidefriend();
    }
    QGraphicsObject::mousePressEvent(event);
}

ShowFriend::~ShowFriend()
{
}



ShowFriendGroup::ShowFriendGroup()
{   
    ShowFriend * showfriend = new ShowFriend();
    addToGroup(showfriend);
    showfriend->setPos(0, 0);

    friendGroup = new FriendGroup(persons);
    addToGroup(friendGroup);
    friendGroup->setPos(0, 0);

    searchGroup = new FriendGroup(searchs);
    addToGroup(searchGroup);
    searchGroup->setPos(0, 0);
    searchGroup->hide();

    showFriend = new ButtonItem(0, 0, 60, 20, "显示好友");
    addToGroup(showFriend);
    showFriend->setPos(20, -240);
    showSearch = new ButtonItem(0, 0, 60, 20, "显示搜索");
    addToGroup(showSearch);
    showSearch->setPos(100, -240);

    connect(showFriend, SIGNAL(clicked()), this, SLOT(showFriendInfor()));
    connect(showSearch, SIGNAL(clicked()), this, SLOT(showSearchInfor()));

    connect(showfriend,SIGNAL(showfriend()),this,SLOT(showScene()));
    connect(showfriend,SIGNAL(hidefriend()),this,SLOT(hideScene()));
}

QRectF ShowFriendGroup::boundingRect() const//我猜是用来确定重绘区域的，但是不确定
{
    return QRectF(-30, -250, 210, 500);
}

void ShowFriendGroup::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    Q_UNUSED(painter);
}

QPainterPath ShowFriendGroup::shape() const//这个函数返回值确定的是这个item的作用范围，比如点击事件,碰撞检测
{
    QPainterPath path;
    path.addRect(0, -250, 180, 500);
    return path;
}

void ShowFriendGroup::showScene()
{
    for(int i=0; i<18; i++)
        moveBy(-10, 0);
}

void ShowFriendGroup::hideScene()
{
    for(int i=0; i<18; i++)
        moveBy(10, 0);
}

void ShowFriendGroup::showFriendInfor()
{
    friendGroup->show();
    searchGroup->hide();
}

void ShowFriendGroup::showSearchInfor()
{
    friendGroup->hide();
    searchGroup->show();
}

void ShowFriendGroup::getFriend(QDataStream &in)
{
    persons.clear();
    int friendsNum;
    in >> friendsNum;
    QString username, faceaddress;
    int level, exp, money;
    while(friendsNum--)
    {
        in >> username >> faceaddress >> level >> exp >> money;
        persons << UserInfo(username, QString(":face/image/face/%1-1.GIF").arg(faceaddress), level, exp, money);
    }
    friendGroup->updateFriend(persons);
    MainView::updateMutex.unlock();
}

ShowFriendGroup::~ShowFriendGroup() {
}



