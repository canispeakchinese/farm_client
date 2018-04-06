#ifndef TOOL_H
#define TOOL_H

#include <QGraphicsObject>
enum ToolType {Spad, Pack, Water, Pyre, Weed, Harv, Alhar, Chat, Plant, Ferti, Empty};

class Tool:public QGraphicsObject {
    Q_OBJECT
public:
    explicit Tool(ToolType type, QString filename);
    virtual QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    ~Tool();

protected:
    bool sceneEvent(QEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
    void sendStatus(ToolType);

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
    void sendTool(Tool * tool);
};

#endif // TOOL_H
