#include "showscenegroup.h"
#include "dal/baseview/buttonitem.h"
#include "store.h"
#include "shop.h"
#include <QPainter>

ShowSceneGroup::ShowSceneGroup(int level) : level(level)
{
}

void ShowSceneGroup::createButton()
{
    ButtonItem * buttonitem;

    //buttonitem = new ButtonItem(-30, -30, 60, 60, QPixmap(":scene/image/scene/decorate.png"));
    //buttonitem->setPos(-30, -30);
    //addToGroup(buttonitem);

    store = new Store(level);
    buttonitem = new ButtonItem(-30, -30, 60, 60, QPixmap(":scene/image/scene/store.png"));
    connect(buttonitem,SIGNAL(clicked()),store,SLOT(show()));
    buttonitem->setPos(-100, -30);
    addToGroup(buttonitem);

    shop = new Shop(level);
    buttonitem = new ButtonItem(-30, -30, 60, 60, QPixmap(":scene/image/scene/shop.png"));
    connect(buttonitem,SIGNAL(clicked()),shop,SLOT(show()));
    buttonitem->setPos(-170, -30);
    addToGroup(buttonitem);
}

QRectF ShowSceneGroup::boundingRect() const//我猜是用来确定刷新区域的，但是不确定
{
    return QRectF(-170, -30, 170, 30);
}

void ShowSceneGroup::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    Q_UNUSED(painter);
}

QPainterPath ShowSceneGroup::shape() const//这个函数返回值确定的是这个item的作用范围，比如点击事件,碰撞检测
{
    QPainterPath path;
    path.addRect(-170, -30, 170, 30);
    return path;
}

void ShowSceneGroup::getGoods(QDataStream &in, Business business) {
    this->store->getGoods(in, business);
    this->shop->getGoods(in, business);
}

ShowSceneGroup::~ShowSceneGroup()
{

}
