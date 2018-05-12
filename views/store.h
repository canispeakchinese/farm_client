#ifndef STORE_H
#define STORE_H

#include "mainview.h"
#include "views/warehouse.h"

class Store : public WareHouse
{
    Q_OBJECT
public:
    explicit Store(int level, QWidget *parent = 0);
    virtual void receiveGoodChange(GoodChange::GoodChangeSource source);
    virtual int updateBusinessResult();
    ~Store();
};

#endif // STORE_H
