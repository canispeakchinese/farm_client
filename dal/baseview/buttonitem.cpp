#include "buttonitem.h"
#include <QPainter>
#include <QEvent>
#include <QMessageBox>

ButtonItem::ButtonItem(qreal x, qreal y, qreal w, qreal h, QPixmap pixmap):x(x), y(y), w(w), h(h), pixmap(pixmap) {
    model = Pixmap;
}

ButtonItem::ButtonItem(qreal x, qreal y, qreal w, qreal h, QString str):x(x), y(y), w(w), h(h), str(str) {
    model = String;
}

//我猜是用来确定刷新区域的，但是不确定
QRectF ButtonItem::boundingRect() const {
    return QRectF(x, y, w, h);
}

void ButtonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    // 为小方块使用贴图
    if(model == Pixmap) {
        painter->drawPixmap(x, y, w, h, pixmap);
    } else if(model == String) {
        painter->setBrush(Qt::yellow);
        painter->setPen(Qt::yellow);
        painter->drawRect(x, y, w, h);
        painter->setPen(Qt::black);
        painter->drawText(x, y, w, h, Qt::AlignHCenter, str);
    }
}

//这个函数返回值确定的是这个item的作用范围，比如点击事件,碰撞检测
QPainterPath ButtonItem::shape() const {
    QPainterPath path;
    path.addRect(x, y, w, h);
    return path;
}

bool ButtonItem::sceneEvent(QEvent *event) {
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


void ButtonItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    emit clicked();
    QGraphicsObject::mousePressEvent(event);
}

ButtonItem::~ButtonItem() {
}

