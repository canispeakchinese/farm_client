#ifndef SHOP_H
#define SHOP_H


#include "warehouse.h"

using namespace std;

class QTabWidget;

class Shop : public WareHouse
{
    Q_OBJECT
public:
    explicit Shop(int level, QWidget *parent = 0);
    virtual void receiveGoodChange(GoodChange::GoodChangeSource source);
    virtual int updateBusinessResult();
    ~Shop();

public slots:
    void moneyChange(int money);
};

#endif // SHOP_H
