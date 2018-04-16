#include "goodgroup.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QTcpSocket>
#include <QDebug>

GoodItem::GoodItem(Business business, Good good, bool levelEnough) : business(business),
    good(good), levelEnough(levelEnough) {
}

QRectF GoodItem::boundingRect() const {
    if(business == Use) {
        return QRectF(-20, -20, 40, 40);
    }
    return QRectF(-40, -48, 80, 98);
}

void GoodItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if(business == Use) {
        if(good.type == Seed) {
            painter->drawPixmap(-18, -18, 36, 36, QPixmap(QString("%1/seed.png").arg(good.address)));
        } else if(good.type == Fertilize) {
            painter->drawPixmap(-18, -18, 36, 36, QPixmap(good.address));
        }
    } else {
        if(good.type == Seed || good.type == Fruit) {
            painter->drawPixmap(-30, -45, 60, 60, QPixmap(QString("%1/seed.png").arg(good.address)));
        } else if(good.type == Fertilize) {
            painter->drawPixmap(-30, -45, 60, 60, QPixmap(good.address));
        }
        if(business == Sell) {
            painter->drawText(QRect(-20, 20, 40, 30), Qt::AlignCenter, QString::number(good.num));
        } else {
            painter->drawText(QRect(-20, 20, 40, 30), Qt::AlignCenter, good.name);
        }
        painter->drawRect(-40, -48, 80, 98);
    }
    //QGraphicsObject::paint(painter, option, widget);
}

QPainterPath GoodItem::shape() const {
    QPainterPath path;
    if(business == Use) {
        path.addRect(-20, -20, 40, 40);
    } else {
        path.addRect(-40, -48, 80, 98);
    }
    return path;
}

bool GoodItem::sceneEvent(QEvent *event)
{
    if(event->type() == QEvent::GraphicsSceneMousePress) {
        mousePressEvent((QGraphicsSceneMouseEvent *)event);
    } else if(event->type() == QEvent::GraphicsSceneMouseRelease) {
        mouseReleaseEvent((QGraphicsSceneMouseEvent *)event);
    } else if(event->type() == QEvent::GraphicsSceneMouseMove) {
        mouseMoveEvent((QGraphicsSceneMouseEvent *)event);
    } else if(event->type() == QEvent::GraphicsSceneMouseDoubleClick) {
        mouseDoubleClickEvent((QGraphicsSceneMouseEvent *)event);
    }
    return QGraphicsObject::sceneEvent(event);
}

void GoodItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(!levelEnough && business != Sell) {
        QMessageBox::warning(NULL, "无法操作", "玩家等级不足，请先提升等级");
        QGraphicsObject::mousePressEvent(event);
        return;
    }
    emit clicked(good);
    QGraphicsObject::mousePressEvent(event);
}

GoodItem::~GoodItem() {
}

GoodGroup::GoodGroup(Business business, set<Good> good, int len, int level) :
    business(business), good(good), len(len), level(level) {
}

void GoodGroup::createGoodItem() {
    foreach (QGraphicsItem * temp, childItems()) {
        removeFromGroup(temp);
        temp->~QGraphicsItem();
    }
    set<Good>::iterator it = good.begin();
    for(int i=0; ; i++) {
        for(int j=0; j!=len; j++) {
            if(it == good.end()) {
                return;
            }
            GoodItem * gooditem = new GoodItem(business, *it, it->buyLevel <= level);
            connect(gooditem, SIGNAL(clicked(Good)), this, SLOT(startBusiness(Good)));
            addToGroup(gooditem);
            if(business == Use) {
                gooditem->setPos(j*50+30, 20);
            } else {
                gooditem->setPos(j*100 + 70, i*100+70);
            }
            it++;
        }
    }
}

void GoodGroup::updateGoodItem(set<Good> newgood) {
    good = newgood;
    createGoodItem();
}

void GoodGroup::updateGoodItem(int _level) {
    level = _level;
    createGoodItem();
}

QRectF GoodGroup::boundingRect() const {
    if(business == Use)
        return QRectF(0, 0, len*50+20, 40);
    int num = good.size()/len;
    if(good.size()%len) {
        num++;
    }
    return QRectF(0, 0, 430, num*100);
}

void GoodGroup::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QGraphicsItemGroup::paint(painter, option, widget);
}

QPainterPath GoodGroup::shape() const {
    QPainterPath path;
    if(business == Use) {
        path.addRect(0, 0, len*50+20, 40);
        return path;
    }
    int num = good.size()/4;
    if(good.size()%4) {
        num++;
    }
    path.addRect(0, 0, 450, num*100);
    return path;
}

void GoodGroup::startBusiness(Good good) {
    if(business == Buy || business == Sell) {
        BusinessDialog * businessdialog = new BusinessDialog(business, good);
        connect(businessdialog,SIGNAL(business_success(Good)),this,SLOT(getInfor(Good)));
        businessdialog->exec();
    } else {
        getInfor(good);
    }
}

void GoodGroup::getInfor(Good good) {
    emit sendRequest(good);
}

GoodGroup::~GoodGroup() {
}
