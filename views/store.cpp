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

void Store::receiveGoodChange(GoodChange::GoodChangeSource source) {
    Good good = MainView::getGood();
    qDebug() << "Store receive good, name: " << good.name << ", num: " << good.num << ", source: " << source;
    if(source == GoodChange::Update) {
        return;
    }
    bool find = false;
    for(set<Good>::iterator it = goods[good.type].begin(); it != goods[good.type].end(); it++) {
        if(it->kind == good.kind) {
            Good newGood = *it;
            goods[good.type].erase(it);
            if(source == GoodChange::Get) {
                newGood.num += good.num;
                goods[good.type].insert(newGood);
            } else if(source == GoodChange::Lose) {
                if(good.num != 0) {
                    goods[good.type].insert(good);
                }
            }
            find = true;
            break;
        }
    }
    if(!find) {
        /* TODO
         * if(source == GoodChange::Lose) fix bug;
         */
        if(source == GoodChange::Get) {
            goods[good.type].insert(good);
        }
    }
    goodgroup[good.type]->updateGoodItem(goods[good.type]);
}

int Store::updateBusinessResult() {
    int moneyChange = 0;
    if(businessGood.type == Seed || businessGood.type == Fertilize) {
        moneyChange = businessGood.buyPrice * 0.8 * businessGood.num;
    } else {
        moneyChange = businessGood.sellPrice * businessGood.num;
    }
    for(set<Good>::iterator it = goods[businessGood.type].begin(); it != goods[businessGood.type].end(); it++) {
        if(it->kind == businessGood.kind) {
            businessGood.num = it->num - businessGood.num;
            MainView::setGood(businessGood);
            break;
        }
    }
    emit goodChange(StoreSource, GoodChange::Lose);
    return moneyChange;
}

Store::~Store()
{

}

