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
        for(set<Good>::iterator it = goods[i].begin(); it!=goods[i].end();)
        {
            good = *it;
            goods[i].erase(it);
            good.num = money/good.buyPrice;
            goods[i].insert(good);
            it++;
        }
        goodgroup[good.type]->updateGoodItem(goods[good.type]);
    }
}

Shop::~Shop()
{

}
