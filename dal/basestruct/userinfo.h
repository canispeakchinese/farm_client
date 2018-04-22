#ifndef USERINFO_H
#define USERINFO_H

#include <QString>

const int levelLimit = 11;
const int expLimit = 9999;
const int needExp[11] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};

class UserInfo
{
public:
    UserInfo(){}
    UserInfo(UserInfo* sourceData) {
        username = sourceData->username;
        faceaddress = sourceData->faceaddress;
        level = sourceData->level;
        exp = sourceData->exp;
        money = sourceData->money;
    }
    UserInfo(QString username, QString faceaddress, int level, int exp, int money);
    UserInfo(const UserInfo &person);
    ~UserInfo();

    QString toString() {
        return "username: " + username + ", faceaddress: " + faceaddress + ", level: " + level + ", exp: " + exp + " , money: " + money;
    }

    QString username;
    QString faceaddress;
    int level;
    int exp;
    int money;
};

#endif
