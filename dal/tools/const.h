#ifndef CONST_H
#define CONST_H

#include <QString>

const QString ipAddress = "192.168.130.128";

enum Business {
    Buy, Sell, Use//买,卖,使用
};

namespace GoodChange {
    enum GoodChangeSource {
        Lose, Get, Update
    }; //失去（此时为剩余的数目），获得（此时为增加的数目)，更新（只是更新mainview中的good的值，然后可能用户种植或者交易)
}

const QString SoilGroupSource = "SoilGroup";
const QString ToolGroupSource = "ToolGroup";
const QString MainViewSource = "MainView";
const QString PackGroupSource = "PackGroup";
const QString ShopSource = "Shop";
const QString StoreSource = "Store";
const QString ShowSceneSource = "ShowScene";

enum ToolType {Spad, Pack, Water, Pyre, Weed, Harv, Alhar, Chat, Plant, Ferti, Empty};

#endif
