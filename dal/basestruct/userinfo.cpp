#include "userinfo.h"

UserInfo::UserInfo(QString username, QString faceaddress, int level, int exp, int money):
    username(username), faceaddress(faceaddress), level(level), exp(exp), money(money) {

}

UserInfo::UserInfo(const UserInfo &person):username(person.username), faceaddress(person.faceaddress),
    level(person.level), exp(person.exp), money(person.money) {

}

UserInfo::~UserInfo() {

}
