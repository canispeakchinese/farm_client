#ifndef WAREHOUSE_H
#define WAREHOUSE_H


#include "dal/baseview/basescene.h"
#include "views/goodgroup.h"
#include <set>

using namespace std;

class QTabWidget;

class WareHouse : public BaseScene
{
    Q_OBJECT
public:
    explicit WareHouse(Business business, int column, int level, QWidget *parent = 0);
    void createScene(GoodType type);
    void business_failed();
    ~WareHouse();

signals:
    void sendBusinessRequest(Business buesiness, Good good, int businessNum);

public slots:
    void getGoods(QDataStream &in, Business business);
    void sendRequest(Good good, int businessNum);
    void levelChange(int _level);

protected:
    Business business;
    int column;   //展示物品的时候有多少列
    int level;
    set<Good> goods[3];
    GoodGroup * goodgroup[3];

    QTabWidget * tabwidget;
};

#endif // WAREHOUSE_H
