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
    ~ShowSceneGroup();

signals:
    void sendWarehouseRequestToServer(QByteArray outBlock);

public slots:
    void getGoods(QDataStream&, Business);
    void getRequestFromWarehouse(QByteArray outBlock) {
        emit sendWarehouseRequestToServer(outBlock);
    }

private:
    Store* store;
    Shop* shop;
    int level;
};

#endif // SHOWSCENEGROUP_H
