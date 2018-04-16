#ifndef SHOWINFORGROUP_H
#define SHOWINFORGROUP_H

#include <QGraphicsItemGroup>

#include "mainview.h"

class ShowInforGroup : public QGraphicsItemGroup
{
public:
    explicit ShowInforGroup(UserInfo person);
    virtual QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;

    void moneyChange(int _money);
    bool expAdd(int _exp);

    ~ShowInforGroup();

private:
    UserInfo person;
};

#endif // SHOWINFORGROUP_H
