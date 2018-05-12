#ifndef PACKGROUP_H
#define PACKGROUP_H

#include "warehouse.h"

using namespace std;

class QTabWidget;

class PackGroup : public WareHouse
{
    Q_OBJECT
public:
    explicit PackGroup(int level, QWidget *parent = 0);
    virtual int updateBusinessResult() {
        return 0;
    }
    ~PackGroup();

public slots:
    virtual void receiveGoodChange(GoodChange::GoodChangeSource source);
    void goodChange(Business business, Good* good);
};

#endif // PACKGROUP_H
