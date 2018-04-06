#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/*任务:
 *     1.显示玩家主界面
 *          1.显示玩家等级，经验，金币等信息;
 *          2.显示所有土地土地情况：    v
 *              1.显示
 *          3.显示工具栏              v
 *          4.显示好友小框            v
 *          5.显示黑名单小框
 *          6.显示消息
 *          7.显示任务
 *     2.显示好友界面
 *          1.显示好友土地
 */

#include <QGraphicsView>
#include <queue>
#include "views/warehouse.h"
#include "dal/baseview/tool.h"

class QGraphicsScene;
class SoilGroup;
class ShowFriendGroup;
class ShowInforGroup;
class Soil;
class PersonDefin;
class ShowSceneGroup;
class Store;
class Shop;
class PackGroup;
class Good;
class QTcpSocket;
class Login;
class QTimer;
class ChatWidget;
class TcpClient;

class MainView : public QGraphicsView
{
    Q_OBJECT

public:
    MainView(QGraphicsView * parent = 0);
    void initMainView();
    void createSoilGroup();
    void createToolGroup();
    void createFriendGroup();
    void createInforGroup();
    void createSceneGroup();
    void createPackGroup();
    void createChatWidget();
    void getUpdateResult(QDataStream &in);
    void getBusinessResult(QDataStream &in);
    void getPlantResult(QDataStream &in);
    void getSpadResult(QDataStream &in);
    void getHarvestResult(QDataStream &in);
    void getStatusChangeResult(QDataStream &in);
    void getReclaResult(QDataStream &in);
    void getFertilizeResult(QDataStream &in);
    ~MainView();

protected:
    void mouseMoveEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent * event);

signals:
    void sendStatus(ToolType type);
    void moneyChange(int money);
    void levelChange(int level);
    void goodChange(Business business, Good good);
    void getFriend(QDataStream &in);
    void getSoils(QDataStream &in);
    void getGoods(QDataStream &in, Business business);
    void plantSuccess(int number, int kind, QDataStream &in);
    void spadSuccess(int number);
    void harvestSuccess(int number);
    void statusChangeSuccess(int number, int _status, bool _auto, QDataStream &in);
    void reclaSuccess();
    void fertilizeSuccess(int number, int reduTime);

public slots:
    void connectSoil(Soil * soil);
    void connectTool(Tool * tool);
    void connectStore(Store * store);
    void connectShop(Shop * shop);
    void receiveStatus(ToolType newtype);
    void connectError();
    void readyRead();
    void sendUpdateRequest(int updateRequest);
    void sendBusinessRequest(Business _business, Good _good, int _businessNum);
    void sendPlantRequest(int _number);
    void sendSpadRequest(int _number);
    void sendHarvestRequest(int _number);
    void sendStatusChangeRequest(int _status, int _number, bool _auto);//土地状态更新
    void sendReclaRequest(int number);
    void sendFertilizeRequest(int number);
    void logInSuccess(QDataStream& in);

private:
    TcpClient* tcpClient2;
    QTcpSocket * tcpClient;
    Login * login;
    int id;                     //玩家编号
    QString username;
    QString password;           //对应密码

    PersonDefin * person;

    QGraphicsScene * scene;

    QGraphicsPixmapItem * showstatus;      //显示当前状态
    SoilGroup * soilgroup;                 //土地
    ToolGroup * toolgroup;                 //工具
    ShowFriendGroup * showfriendgroup;     //好友
    ShowInforGroup * showinforgroup;       //个人信息
    ShowSceneGroup * showscenegroup;       //商店等窗口
    PackGroup * packgroup;                 //背包
    ChatWidget * chatWidget;               //聊天界面

    ToolType toolType;
    qint64 totalBytes;
    QByteArray inBlock;
    int messageType;                         //消息的类型
    //int updateRequest;                     //更新的范围
    //int number;                            //当前操作的土地
    //bool isAuto;                           //当前操作是否是自动执行的

    Business business;
    Good good;
    int businessNum;
};

#endif // MAINWINDOW_H
