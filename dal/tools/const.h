#ifndef CONST_H
#define CONST_H

#include <QString>

const QString ipAddress = "192.168.199.183";

enum Business {
    Buy, Sell, Use//买,卖,使用
};
const QString SoilGroupSource = "SoilGroup";
const QString ToolGroupSource = "ToolGroup";
const QString MainViewSource = "MainView";
const QString PackGroupSource = "PackGroup";
const QString ShopSource = "Shop";
const QString StoreSource = "Store";

enum ToolType {Spad, Pack, Water, Pyre, Weed, Harv, Alhar, Chat, Plant, Ferti, Empty};

#endif
