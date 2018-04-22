#ifndef SHOWINFORGROUP_H
#define SHOWINFORGROUP_H

#include <QGraphicsItemGroup>

#include "mainview.h"

class ShowInforGroup : public QObject, public QGraphicsItemGroup {
    Q_OBJECT
public:
    explicit ShowInforGroup(UserInfo person);
    virtual QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;

    ~ShowInforGroup();

public slots:
    void receiveUserInfoChange();

private:
    UserInfo person;
};

#endif // SHOWINFORGROUP_H
