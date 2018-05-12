#include "showscenegroup.h"
#include "dal/baseview/buttonitem.h"
#include "store.h"
#include "shop.h"
#include <QPainter>
#include <QMessageBox>

ShowSceneGroup::ShowSceneGroup(int level) : level(level) {
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

    connect(store, SIGNAL(sendBusinessRequest(QByteArray, Business)), this, SLOT(getRequestFromWarehouse(QByteArray, Business)));
    connect(shop, SIGNAL(sendBusinessRequest(QByteArray, Business)), this, SLOT(getRequestFromWarehouse(QByteArray, Business)));

    connect(store, SIGNAL(goodChange(QString,GoodChange::GoodChangeSource)), this, SLOT(getGoodChangeFromWarehouse(QString,GoodChange::GoodChangeSource)));
    connect(shop, SIGNAL(goodChange(QString,GoodChange::GoodChangeSource)), this, SLOT(getGoodChangeFromWarehouse(QString,GoodChange::GoodChangeSource)));
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

void ShowSceneGroup::receiveGoodChange(GoodChange::GoodChangeSource source) {
    this->store->receiveGoodChange(source);
    this->shop->receiveGoodChange(source);
}

void ShowSceneGroup::getBusinessResult(QDataStream &in) {
    int messageType;
    in >> messageType;
    if(messageType == 0)
        QMessageBox::warning(parent, "交易失败", "交易数目不合法");
    else if(messageType == 1)
        QMessageBox::warning(parent, "交易失败", "交易信息错误");
    else if(messageType == 2)
        QMessageBox::warning(parent, "交易失败", "交易数目不合理");
    else if(messageType == 3) {
        UserInfo userInfo = MainView::getUserInfo();
        if(business == Buy) {
            userInfo.money += shop->updateBusinessResult();
        } else if(business == Sell) {
            userInfo.money += store->updateBusinessResult();
        }
        MainView::setUserInfo(userInfo);
        emit moneyChange(ShowSceneSource);
    }
}

void ShowSceneGroup::receiveUserInfoChange() {
    UserInfo info = MainView::getUserInfo();
    shop->moneyChange(info.money);
    if(info.level == level) {
        return;
    }
    level = info.level;
    shop->levelChange(level);
    store->levelChange(level);
}

ShowSceneGroup::~ShowSceneGroup()
{

}
