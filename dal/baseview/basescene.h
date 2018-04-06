#ifndef BASESCENE_H
#define BASESCENE_H


#include <QGraphicsView>

class BaseScene : public QGraphicsView
{
    Q_OBJECT
public:
    explicit BaseScene(QWidget *parent = 0);
    ~BaseScene();

signals:

public slots:
};

#endif // BASESCENE_H
