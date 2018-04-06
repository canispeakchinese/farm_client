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
    dal/baseview/basescene.cpp \
    dal/baseview/buttonitem.cpp \
    dal/baseview/tool.cpp \
    dal/network/tcpclient.cpp \
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
    mainview.cpp

HEADERS  += \
    dal/baseview/basescene.h \
    dal/baseview/buttonitem.h \
    dal/baseview/tool.h \
    dal/network/tcpclient.h \
    tools/const.h \
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
    mainview.h

RESOURCES += \
    image.qrc

FORMS +=
