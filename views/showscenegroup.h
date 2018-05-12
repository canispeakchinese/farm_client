#ifndef SHOWSCENEGROUP_H
#define SHOWSCENEGROUP_H

#include "mainview.h"

#include <QGraphicsItemGroup>
#include <QObject>

class Store;
class Shop;

class ShowSceneGroup : public QObject , public QGraphicsItemGroup
{
    Q_OBJECT
public:
    explicit ShowSceneGroup(int level);
    void createButton();
    virtual QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    void setParent(QWidget* widget) {
        parent = widget;
    }
    ~ShowSceneGroup();

signals:
    void sendWarehouseRequestToServer(QByteArray outBlock);
    void goodChange(QString source, GoodChange::GoodChangeSource goodChangeSource);
    void moneyChange(QString source);

public slots:
    void getGoods(QDataStream&, Business);
    void getBusinessResult(QDataStream&);
    void receiveGoodChange(GoodChange::GoodChangeSource source);
    void receiveUserInfoChange();
    void getRequestFromWarehouse(QByteArray outBlock, Business _business) {
        business = _business;
        emit sendWarehouseRequestToServer(outBlock);
    }
    void getGoodChangeFromWarehouse(QString source, GoodChange::GoodChangeSource goodChangeSource) {
        emit goodChange(source, goodChangeSource);
    }

private:
    QWidget* parent;
    Store* store;
    Shop* shop;
    Business business;
    int level;
};

#endif // SHOWSCENEGROUP_H
