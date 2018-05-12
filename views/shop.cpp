#include "shop.h"
#include "dal/baseview/buttonitem.h"
#include <QGraphicsScene>
#include <QTabWidget>
#include <QMessageBox>
#include <QDebug>

Shop::Shop(int level, QWidget *parent) :
    WareHouse(ShopSource, Buy, 4, level, parent)
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

void Shop::moneyChange(int money)
{
    Good good;
    for(int i=0; i<3; i++)
    {
        if(i == Fruit)
            continue;
        set<Good> newGoods;
        for(set<Good>::iterator it = goods[i].begin(); it!=goods[i].end(); it++)
        {
            good = *it;
            good.num = money/good.buyPrice;
            newGoods.insert(good);
        }
        goods[i] = newGoods;
        goodgroup[good.type]->updateGoodItem(goods[good.type]);
    }
}

void Shop::receiveGoodChange(GoodChange::GoodChangeSource source) {
    Q_UNUSED(source);
    return;
}

int Shop::updateBusinessResult() {
    MainView::setGood(businessGood);
    emit goodChange(ShopSource, GoodChange::Get);
    return -businessGood.buyPrice * businessGood.num;
}

Shop::~Shop()
{

}
