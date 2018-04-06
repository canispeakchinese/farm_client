#ifndef GOODGROUP_H
#define GOODGROUP_H

#include "businessdialog.h"
#include <QGraphicsObject>
#include <set>

using namespace std;

class QTcpSocket;

class GoodItem :public QGraphicsObject
{
    Q_OBJECT
public:
    explicit GoodItem(Business business, Good good, bool levelEnough);
    virtual QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    ~GoodItem();

signals:
    void clicked(Good good);

protected:
    bool sceneEvent(QEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    Business business;
    Good good;
    bool levelEnough;
};

class GoodGroup : public QObject , public QGraphicsItemGroup
{
    Q_OBJECT
public:
    explicit GoodGroup(Business business, set<Good> good,int len, int level);//len-每一行有多长,默认无限长
    void createGoodItem();
    void updateGoodItem(set<Good> newgood);
    void updateGoodItem(int _level);
    virtual QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    ~GoodGroup();

signals:
    void sendRequest(Good good, int businessNum);

protected slots:
    void startBusiness(Good good);
    void getInfor(Good good, int businessNum);

private:
    Business business;
    set<Good> good;
    int len;
    int level;
};

#endif // GOODGROUP_H
