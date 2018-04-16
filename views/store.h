#ifndef STORE_H
#define STORE_H

#include "mainview.h"
#include "views/warehouse.h"

class Store : public WareHouse
{
    Q_OBJECT
public:
    explicit Store(int level, QWidget *parent = 0);
    ~Store();

public slots:
    void goodChange(Business business, Good good);
    void getBusinessResult(QDataStream& in);
};

#endif // STORE_H
