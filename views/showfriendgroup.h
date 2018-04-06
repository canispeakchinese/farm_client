#ifndef SHOWFRIENDGROUP_H
#define SHOWFRIENDGROUP_H

#include <QGraphicsObject>
#include <QObject>

class FriendGroup;
class PersonDefin;
class ButtonItem;

class ShowFriend : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit ShowFriend();
    virtual QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    ~ShowFriend();

protected:
    bool sceneEvent(QEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
    void showfriend();
    void hidefriend();

private:
    bool is_show;     //当前状态
};

class ShowFriendGroup : public QObject , public QGraphicsItemGroup
{
    Q_OBJECT
public:
    explicit ShowFriendGroup();
    virtual QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    ~ShowFriendGroup();

public slots:
    void showScene();
    void hideScene();
    void showFriendInfor();
    void showSearchInfor();
    void getFriend(QDataStream &in);

private:
    FriendGroup * friendGroup;
    FriendGroup * searchGroup;
    QVector<PersonDefin> persons;
    QVector<PersonDefin> searchs;

    ButtonItem * showFriend;
    ButtonItem * showSearch;
};

#endif // SHOWFRIENDGROUP_H
