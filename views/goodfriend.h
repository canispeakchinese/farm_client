#ifndef GOODFRIEND_H
#define GOODFRIEND_H

#include <QGraphicsObject>
#include "showinforgroup.h"

class ButtonItem;

class GoodFriend : public QGraphicsObject
{
public:
    explicit GoodFriend(int number, UserInfo person);
    virtual QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    ~GoodFriend();

protected:
    bool sceneEvent(QEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    int number;
    UserInfo person;
};

class FriendGroup : public QObject , public QGraphicsItemGroup
{
    Q_OBJECT
public:
    explicit FriendGroup(QVector<UserInfo> persons);
    virtual QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    ~FriendGroup();
    void addshowfriend();
    void removeshowfriend();
    void updateFriend(QVector<UserInfo> _persons);

public slots:
    void next();
    void front();

private:
    QVector<UserInfo> persons;
    QList<GoodFriend *> items;
    ButtonItem * left;
    ButtonItem * right;
    int pagenum;
    int currpage;
};

#endif // GOODFRIEND_H
