#include "store.h"
#include "dal/baseview/buttonitem.h"
#include <QGraphicsScene>
#include <QTabWidget>
#include <QMessageBox>
#include <QDebug>

Store::Store(int level, QWidget *parent) :
    WareHouse(Sell, 4, level, parent)
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

Store::~Store()
{

}

