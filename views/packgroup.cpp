#include "packgroup.h"
#include "dal/baseview/buttonitem.h"
#include <QGraphicsScene>
#include <QTabWidget>
#include <QMessageBox>
#include <QDebug>
#include <QTabBar>

PackGroup::PackGroup(int level, QWidget *parent) :
    WareHouse(PackGroupSource, Use, -1, level, parent)
{
    setMinimumSize(700, 120);
    setMaximumSize(700, 120);

    QPushButton * pushbutton = new QPushButton(QIcon(QPixmap(QString(":guiRes/image/guiRes/5.png"))),
                                               QString(""),this);
    pushbutton->setIconSize(QSize(20,20));
    connect(pushbutton,SIGNAL(clicked(bool)),this,SLOT(hide()));
    pushbutton->setGeometry(670, 10, 20, 20);

    tabwidget->setGeometry(10, 10, 680, 100);
}

void PackGroup::goodChange(Business business, Good *good)
{
    if(good->type == Fruit)
        return;
    set<Good>::iterator it = goods[good->type].find(*good);
    if(it != goods[good->type].end())
    {
        if(business == Buy)
            good->num += it->num;
        goods[good->type].erase(it);
    }
    if(good->num)
        goods[good->type].insert(*good);
    goodgroup[good->type]->updateGoodItem(goods[good->type]);
}

PackGroup::~PackGroup() {
}
