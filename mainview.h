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
#include <QMutex>

#include "dal/basestruct/userinfo.h"
#include "dal/basestruct/good.h"
#include "dal/tools/const.h"
#include "dal/tools/utils.h"

class QGraphicsScene;
class SoilGroup;
class ShowFriendGroup;
class ShowInforGroup;
class UserInfo;
class ShowSceneGroup;
class PackGroup;
class Good;
class Login;
class ChatWidget;
class TcpClient;
class ToolGroup;

class MainView : public QGraphicsView
{
    Q_OBJECT

public:
    MainView(QGraphicsView* parent = 0);
    void start();

    static ToolType getStatus() {
        statusMutex.lock();
        ToolType type = toolType;
        statusMutex.unlock();
        return type;
    }
    static void setStatus(ToolType type) {
        statusMutex.lock();
        toolType = type;
        statusMutex.unlock();
    }

    static UserInfo getUserInfo() {
        userInfoMutex.lock();
        UserInfo userInfo = person;
        userInfoMutex.unlock();
        return userInfo;
    }
    static void setUserInfo(UserInfo userInfo) {
        userInfoMutex.lock();
        person = userInfo;
        userInfoMutex.unlock();
    }

    static Good getGood() {
        goodMutex.lock();
        Good item = good;
        goodMutex.unlock();
        return item;
    }
    static void setGood(Good item) {
        goodMutex.lock();
        good = item;
        goodMutex.unlock();
    }
    ~MainView();

protected:
    void mouseMoveEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent * event);

signals:
    void statusChange(QString source);
    void sendStatusChange();

    void moneyChange(QString source);
    void sendMoneyChange();

    void expChange(QString source);
    void sendExpChange();

    void sendGoodChange(GoodChange::GoodChangeSource);

    void sendUpdateRequest(QByteArray);
    void getSoils(QDataStream& in);
    void getFriend(QDataStream& in);
    void getGoods(QDataStream& in, Business business);

public slots:
    void statusChangeCenter(QString source);
    void moneyChangeCenter(QString source);
    void expChangeCenter(QString source);
    void goodChangeCenter(QString source, GoodChange::GoodChangeSource goodChangeSource);

    void logInSuccess(QDataStream& in);
    void getUpdateResult(QDataStream &in);

private:
    void initMainView();
    void createSoilGroup();
    void createToolGroup();
    void createFriendGroup();
    void createInforGroup();
    void createSceneGroup();
    void createPackGroup();
    void createChatWidget();

    QByteArray createUpdateRequest(int request);
    void statusChangeSignalConnect();
    void moneyChangeSignalConnect();
    void expChangeSignalConnect();
    void goodChangeSignalConnect();

    void networkMessageConnectSignalConnect();

    TcpClient* tcpClient;
    Login* login;

    QGraphicsScene* scene;
    QGraphicsPixmapItem* showstatus;      //显示当前状态
    SoilGroup* soilgroup;                 //土地
    ToolGroup* toolgroup;                 //工具
    ShowFriendGroup* showfriendgroup;     //好友
    ShowInforGroup* showinforgroup;       //个人信息
    ShowSceneGroup* showscenegroup;       //商店等窗口
    PackGroup* packgroup;                 //背包
    ChatWidget* chatWidget;               //聊天界面
    QString source;

public:
    static QString username;
    static QString password;
    static ToolType toolType;
    static UserInfo person;
    static Good good;
    static QMutex updateMutex;
    static QMutex statusMutex;
    static QMutex tcpMutex;
    static QMutex userInfoMutex;
    static QMutex goodMutex;
};

#endif // MAINWINDOW_H
