#include "store.h"
#include "dal/baseview/buttonitem.h"
#include <QGraphicsScene>
#include <QTabWidget>
#include <QMessageBox>
#include <QDebug>

Store::Store(int level, QWidget *parent) :
    WareHouse(StoreSource, Sell, 4, level, parent)
{
    setMinimumSize(600, 500);
    setMaximumSize(600, 500);

    QPushButton * pushbutton = new QPushButton(QIcon(QPixmap(QString(":guiRes/image/guiRes/5.png"))),
                                               QString(""),this);
    pushbutton->setIconSize(QSize(35,35));
    connect(pushbutton,SIGNAL(clicked(bool)),this,SLOT(hide()));
    pushbutton->setGeometry(550, 10, 40, 40);

    tabwidget->setGeometry(70, 50, 460, 400);
}

void Store::goodChange(Business business, Good good)
{
    set<Good>::iterator it = goods[good.type].find(good);
    if(it != goods[good.type].end())
    {
        if(business == Buy)
            good.num += it->num;
        goods[good.type].erase(it);
    }
    if(good.num)
        goods[good.type].insert(good);
    goodgroup[good.type]->updateGoodItem(goods[good.type]);
}

void Store::getBusinessResult(QDataStream &in) {
    int messageType;
    in >> messageType;
    if(messageType == 0)
        QMessageBox::warning(this, "交易失败", "交易数目不合法");
    else if(messageType == 1)
        QMessageBox::warning(this, "交易失败", "交易信息错误");
    else if(messageType == 2)
        QMessageBox::warning(this, "交易失败", "交易数目不合理");
    else if(messageType == 3) {
        if(business == Buy) {
            emit moneyChange(-businessGood.buyPrice * businessGood.num);
        } else if(business == Sell) {
            if(businessGood.type == Seed || businessGood.type == Fertilize) {
                emit moneyChange(businessGood.buyPrice * 0.8 * businessGood.num);
            } else {
                emit moneyChange(businessGood.sellPrice * businessGood.num);
            }
        }
        emit goodChange(business, businessGood);
    }
}

Store::~Store()
{

}

