#ifndef TOOL_H
#define TOOL_H

#include "mainview.h"
#include <QGraphicsObject>

class Tool:public QGraphicsObject {
    Q_OBJECT
public:
    explicit Tool(ToolType type, QString filename);
    virtual QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    ~Tool();

signals:
    void statusChange(ToolType*);

protected:
    bool sceneEvent(QEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    ToolType type;
    QString filename;
};

class ToolGroup : public QObject , public QGraphicsItemGroup {
    Q_OBJECT
public:
    explicit ToolGroup();
    void createTool();
    virtual QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    ~ToolGroup();

signals:
    void statusChange(QString source);

public slots:
    void getStatusChange(ToolType*);
    void receiveStatusChange();

private:
    QString source;
};

#endif // TOOL_H
