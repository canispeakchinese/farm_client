#include "warehouse.h"
#include "dal/baseview/buttonitem.h"
#include <QGraphicsScene>
#include <QTabWidget>
#include <QMessageBox>
#include <QDebug>

WareHouse::WareHouse(QString source, Business business, int column, int level, QWidget *parent) :
    BaseScene(parent), source(source), business(business), column(column), level(level),
    tabwidget(new QTabWidget(this))
{
    goodgroup[0] = NULL;
    goodgroup[1] = NULL;
    goodgroup[2] = NULL;
}

void WareHouse::createScene(GoodType type)
{
    QGraphicsView * view = new QGraphicsView();
    view->setBackgroundBrush(QBrush(Qt::yellow));
    QGraphicsScene * scene = new QGraphicsScene();

    view->setScene(scene);

    goodgroup[type] = new GoodGroup(business, goods[type], column, level);
    connect(goodgroup[type], SIGNAL(sendRequest(Good)), this, SLOT(getRequestFromGoodGroup(Good)));
    scene->addItem(goodgroup[type]);
    goodgroup[type]->setPos(25, 25);
    goodgroup[type]->createGoodItem();

    if(type == Seed)
        tabwidget->addTab(view, QIcon(), "种子");
    else if(type == Fruit)
        tabwidget->addTab(view, QIcon(), "果实");
    else if(type == Fertilize)
        tabwidget->addTab(view, QIcon(), "化肥");
}

void WareHouse::business_failed()
{
    QMessageBox * message = new QMessageBox(this);
    if(business == Use)
        message->setText("数据异常,使用失败,请重试");
    else
        message->setText("数据异常,交易失败,请重试!");
    message->exec();
}

void WareHouse::getGoods(QDataStream &in, Business business)
{
    if(business != this->business)
        return;
    goods[0].clear();
    goods[1].clear();
    goods[2].clear();
    int goodNum, money;
    in >> goodNum;
    if(business == Buy)
        in >> money;
    int goodType, kind, num;
    while(goodNum--)
    {
        in >> goodType >> kind;
        if(business == Buy) {
            Good good = createGood((GoodType)goodType, kind, 0);
            good.num = money/good.buyPrice;
            goods[goodType].insert(good);
        } else {
            in >> num;
            goods[goodType].insert(createGood((GoodType)goodType, kind, num));
        }
    }
    createScene(Seed);
    if(business == Sell)
        createScene(Fruit);
    createScene(Fertilize);
    MainView::updateMutex.unlock();
}

void WareHouse::levelChange(int _level)
{
    level = _level;
    for(int i=0; i<2; i++)
    {
        if(goodgroup[i] != NULL)
            goodgroup[i]->updateGoodItem(level);
    }
}

void WareHouse::getRequestFromGoodGroup(Good good) {
    businessGood = good;
    qDebug() << "WareHouse Start Business, Set Status -> " << Empty << ", And Source is " << source;
    MainView::setStatus(Empty);
    emit statusChange(source);
    if(business == Use) {
        MainView::setGood(good);
        emit goodChange(source, GoodChange::Update);
        if(businessGood.type == Seed) {
            MainView::setStatus(Plant);
        } else if(businessGood.type == Fertilize) {
            MainView::setStatus(Ferti);
        }
        qDebug() << "WareHouse Start Use Good, Set Good -> " << good.toString() << ", Set Status -> " << MainView::getStatus() << ", And Source is " << source;
        emit statusChange(source);
        this->hide();
    } else {
        QByteArray outBlock;
        QDataStream out(&outBlock, QIODevice::ReadWrite);
        out << qint64(0) << 5 << (int)business << (int)good.type << (int)good.kind << good.num;
        out.device()->seek(0);
        out << (qint64)outBlock.size();
        emit sendBusinessRequest(outBlock, business);
    }
}

WareHouse::~WareHouse() {
}

