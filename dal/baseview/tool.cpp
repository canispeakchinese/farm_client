#include "tool.h"
#include <QPainter>
#include <QEvent>
#include <QDebug>

Tool::Tool(ToolType type, QString filename) : type(type), filename(filename) {
}

QRectF Tool::boundingRect() const {
    return QRectF(-30, -30, 60, 60);
}

void Tool::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawPixmap(-30, -30, 60, 60, QPixmap(filename));
}

QPainterPath Tool::shape() const {
    QPainterPath path;
    path.addRect(-30, -30, 60, 60);
    return path;
}

bool Tool::sceneEvent(QEvent *event) {
    if(event->type() == QEvent::GraphicsSceneMousePress) {
        mousePressEvent((QGraphicsSceneMouseEvent *)event);
    } else if(event->type() == QEvent::GraphicsSceneMouseRelease) {
        mouseReleaseEvent((QGraphicsSceneMouseEvent *)event);
    } else if(event->type() == QEvent::GraphicsSceneMouseMove) {
        mouseMoveEvent((QGraphicsSceneMouseEvent *)event);
    } else if(event->type() == QEvent::GraphicsSceneMouseDoubleClick) {
        mouseDoubleClickEvent((QGraphicsSceneMouseEvent *)event);
    }
    return true;
}

void Tool::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit statusChange(&type);
    QGraphicsObject::mousePressEvent(event);
}

Tool::~Tool() {
}

ToolGroup::ToolGroup() {
    source = ToolGroupSource;
}

void ToolGroup::createTool() {
    for(int i=0; i<8; i++) {
        Tool *tool = new Tool((ToolType) i, QString(":icon/image/icon/topbutton(")+QString::number(i)+QString(").png"));
        addToGroup(tool);
        tool->setPos(QPoint(30+i*70, 10));
        connect(tool, SIGNAL(statusChange(ToolType*)), this, SLOT(getStatusChange(ToolType*)));
    }
}

QRectF ToolGroup::boundingRect() const {
    return QRectF(0,-20,560,60);
}

void ToolGroup::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    Q_UNUSED(painter);
}

QPainterPath ToolGroup::shape() const {
    QPainterPath path;
    path.addRect(0, 0, 610, 80);
    return path;
}

void ToolGroup::getStatusChange(ToolType* toolType) {
    qDebug() << "ToolGroup Click Tool, Status Change: " << MainView::getStatus() << " -> " << *toolType;
    MainView::setStatus(*toolType);
    emit statusChange(source);
}

void ToolGroup::receiveStatusChange() {

}

ToolGroup::~ToolGroup(){
}

