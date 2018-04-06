#ifndef BUTTONITEM_H
#define BUTTONITEM_H

#include <QGraphicsObject>

class ButtonItem : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit ButtonItem(qreal x, qreal y, qreal w, qreal h, QPixmap pixmap);
    explicit ButtonItem(qreal x, qreal y, qreal w, qreal h, QString str);
    virtual QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    ~ButtonItem();

protected:
    bool sceneEvent(QEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
    void clicked();

private:
    enum Model {Pixmap, String};
    qreal x, y, w, h;
    QPixmap pixmap;
    QString str;
    Model model;
};

#endif // BUTTONITEM_H
