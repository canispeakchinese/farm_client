#-------------------------------------------------
#
# Project created by QtCreator 2016-10-21T23:41:19
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = farm
TEMPLATE = app


SOURCES += \
    views/businessdialog.cpp \
    views/chatwidget.cpp \
    views/goodfriend.cpp \
    views/goodgroup.cpp \
    views/login.cpp \
    views/packgroup.cpp \
    views/shop.cpp \
    views/showfriendgroup.cpp \
    views/showinforgroup.cpp \
    views/showscenegroup.cpp \
    views/soil.cpp \
    views/store.cpp \
    views/warehouse.cpp \
    main.cpp \
    mainview.cpp \
    dal/basestruct/userinfo.cpp \
    dal/baseview/basescene.cpp \
    dal/baseview/buttonitem.cpp \
    dal/baseview/tool.cpp \
    dal/network/tcpclient.cpp \
    dal/basestruct/good.cpp \
    dal/tools/utils.cpp

HEADERS  += \
    views/businessdialog.h \
    views/chatwidget.h \
    views/goodfriend.h \
    views/goodgroup.h \
    views/login.h \
    views/packgroup.h \
    views/shop.h \
    views/showfriendgroup.h \
    views/showinforgroup.h \
    views/showscenegroup.h \
    views/soil.h \
    views/store.h \
    views/warehouse.h \
    mainview.h \
    dal/basestruct/good.h \
    dal/basestruct/userinfo.h \
    dal/baseview/basescene.h \
    dal/baseview/buttonitem.h \
    dal/baseview/tool.h \
    dal/network/tcpclient.h \
    dal/tools/const.h \
    dal/tools/utils.h

RESOURCES += \
    image.qrc

FORMS +=
