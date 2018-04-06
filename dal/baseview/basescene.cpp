#include "basescene.h"
#include "buttonitem.h"
#include <QGraphicsScene>
#include <QPushButton>

BaseScene::BaseScene(QWidget *parent) : QGraphicsView(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowModality(Qt::ApplicationModal);
}

BaseScene::~BaseScene()
{

}
