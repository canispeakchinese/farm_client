#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include "mainview.h"
#include "dal/baseview/basescene.h"
#include "views/goodgroup.h"
#include "dal/baseview/tool.h"
#include <set>

using namespace std;

class QTabWidget;

class WareHouse : public BaseScene
{
    Q_OBJECT
public:
    explicit WareHouse(QString source, Business business, int column, int level, QWidget *parent = 0);
    void createScene(GoodType type);
    void business_failed();
    virtual void receiveGoodChange(GoodChange::GoodChangeSource) = 0;
    virtual int updateBusinessResult() = 0;
    ~WareHouse();

signals:
    void sendBusinessRequest(QByteArray, Business);
    void moneyChange(int money);
    void statusChange(QString source);
    void goodChange(QString source, GoodChange::GoodChangeSource);

public slots:
    void getGoods(QDataStream &in, Business business);
    void getRequestFromGoodGroup(Good good);
    void levelChange(int _level);
    void receiveUserInfoChange() {
        UserInfo info = MainView::getUserInfo();
        if(info.level != level) {
            levelChange(info.level);
        }
    }

protected:
    QString source;
    Good businessGood;
    Business business;
    int column;   //展示物品的时候有多少列
    int level;
    set<Good> goods[3];
    GoodGroup * goodgroup[3];

    QTabWidget * tabwidget;
};

#endif // WAREHOUSE_H
