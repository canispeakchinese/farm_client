#ifndef SHOWINFORGROUP_H
#define SHOWINFORGROUP_H

#include <QGraphicsItemGroup>

class PersonDefin
{
public:
    PersonDefin(){}
    PersonDefin(QString username, QString faceaddress, int level, int exp, int money);
    PersonDefin(const PersonDefin &person);
    ~PersonDefin();

    QString username;
    QString faceaddress;
    int level;
    int exp;
    int money;
};

class ShowInforGroup : public QGraphicsItemGroup
{
public:
    explicit ShowInforGroup(PersonDefin *person);
    virtual QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;

    void moneyChange(int _money);
    bool expAdd(int _exp);

    ~ShowInforGroup();

private:
    PersonDefin *person;
};

#endif // SHOWINFORGROUP_H
