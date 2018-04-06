#ifndef SOIL_H
#define SOIL_H

#include <QGraphicsItem>
#include <QDateTime>
#include <QTimer>
#include "dal/baseview/tool.h"
#include "goodgroup.h"

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
    ~Soil();

signals:
    void sendStatusChangeRequest(int _status, int _number, bool _auto = true);
    //_status 1：water;2:pyre;3：weed. _auto:是否时间到了自动消除
    void sendPlantRequest(int number);
    void sendSpadRequest(int number);
    void sendHarvestRequest(int number);
    void sendFertilizeRequest(int number);

public slots:
    void receiveStatus(ToolType type);
    void update2();
    void matureChange();
    void waterChange();
    void pyreChange();
    void weedChange();
    void plantSuccess(int _num, int _kind, QDataStream &in);
    void spadSuccess(int _num);
    void harvestSuccess(int _num);
    void statusChangeSuccess(int _num, int changeStatus, bool _auto, QDataStream &in);
    void reclaSuccess();
    void fertilizeSuccess(int _num, int reduTime);

protected:
    bool sceneEvent(QEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
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
    ToolType status;               //当前手上有什么工具,比如杀虫剂,种子
    QTimer *timer;                 //重绘定时器
};

class SoilGroup : public QObject , public QGraphicsItemGroup
{
    Q_OBJECT
public:
    explicit SoilGroup();
    void createSoil();
    virtual QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    ~SoilGroup();

signals:
    void sendSoil(Soil * soil);
    void sendReclaRequest(int number);

protected slots:
    void getSoils(QDataStream &in);
    void tryRecla();
    void reclaSuccess();

private:
    int reclaNum;
    ButtonItem *reclaButton;
    Soil *soil[18];
};

#endif // SOIL_H
