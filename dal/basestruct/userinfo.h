#ifndef USERINFO_H
#define USERINFO_H

#include <QString>

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
