#ifndef SOIL_H
#define SOIL_H

#include <QGraphicsItem>
#include <QDateTime>
#include <QTimer>
#include "dal/baseview/tool.h"
#include "goodgroup.h"
#include "mainview.h"

class ButtonItem;

class Soil : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Soil(int num);
    void updateInfor(QDataStream &in);
    virtual QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;

    void plantSuccess(int _kind, QDataStream &in);
    void spadSuccess();
    void harvestSuccess();
    void statusChangeSuccess(int changeStatus, bool _auto, QDataStream &in);
    void reclaSuccess();
    void fertilizeSuccess(int reduTime);

    ~Soil();

signals:
    void sendStatusChangeRequest(QByteArray);
    //_status 1：water;2:pyre;3：weed. _auto:是否时间到了自动消除
    void sendPlantRequest(QByteArray);
    void sendSpadRequest(QByteArray);
    void sendHarvestRequest(QByteArray);
    void getFertilize(int num);

public slots:
    void update2();
    void matureChange();
    void waterChange();
    void pyreChange();
    void weedChange();

protected:
    bool sceneEvent(QEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    QByteArray createStatusChangeRequest(int status, bool _auto=true);

    int num;                       //编号
    int level;                     //土地等级
    int kind;                      //种植的是什么
    int growTime;                  //生长周期
    int allSta;                    //状态数目
    int yield;                     //当前产量
    QTimer *mature;                //果实成熟计时器
    QTimer *water;                 //浇水或缺水剩余时间计时器(及时浇水产量增加）
    QTimer *pyre;                  //除虫或出现害虫剩余时间计时器（不及时除虫产量减少）
    QTimer *weed;                  //出现杂草的时间（暂时不用了）
    int currSta;                   //当前状态
    bool isHarvest;                //是否已收获
    bool isRecla;                  //是否被开垦了
    bool isMature;                 //是否已成熟
    bool need[10];                 //当前状态需要什么,比如除虫,种植
    QTimer *timer;                 //重绘定时器
};

class SoilGroup : public QObject , public QGraphicsItemGroup
{
    Q_OBJECT
public:
    explicit SoilGroup(MainView* parent = 0);
    void createSoil();
    virtual QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    ~SoilGroup();

signals:
    void sendSoilRequestToServer(QByteArray);

    void goodChange(QString source, GoodChange::GoodChangeSource);
    void statusChange(QString source);
    void expChange(QString source);
    void moneyChange(QString source);

protected slots:
    void getSoils(QDataStream &in);
    void tryRecla();
    void reclaSuccess();
    void getPlantResult(QDataStream& in);
    void getSpadResult(QDataStream& in);
    void getHarvestResult(QDataStream &in);
    void getStatusChangeResult(QDataStream &in);
    void getReclaResult(QDataStream &in);
    void getFertilizeResult(QDataStream &in);

    void fertilizeEvent(int num);

private:
    MainView* parent;

    int reclaNum;
    ButtonItem *reclaButton;
    Soil *soil[18];
    QString source;
};

#endif // SOIL_H
