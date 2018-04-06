#ifndef BUSINESSDIALOG_H
#define BUSINESSDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QPaintEvent>

enum GoodType {
    Seed, Fruit, Fertilize
};
enum Business {
    Buy, Sell, Use//买,卖,使用
};

class QLineEdit;
class QGraphicsView;

class Good
{
public:
    Good(){}
    Good(QString name, QString address, GoodType type, int kind, int buyPrice, int buyLevel, int sellPrice, int num = 0) :
        name(name), address(address), type(type), kind(kind), buyPrice(buyPrice),
        buyLevel(buyLevel), sellPrice(sellPrice), num(num)
    {
        if(type == Seed || type == Fruit)
            this->address = QString(":crops/image/crops/crop") + address;
        else if(type == Fertilize)
            this->address = QString(":fertilize/image/fertilize/") + address + ".png";
    }

    Good(const Good &good) : name(good.name), address(good.address), type(good.type), kind(good.kind), buyPrice(good.buyPrice),
        buyLevel(good.buyLevel), sellPrice(good.sellPrice), num(good.num){}
    bool operator <(const Good &good) const
    {
        if(type == good.type)
            return kind < good.kind;
        return type < good.type;
    }
    bool operator ==(const Good &good) const
    {
        return ((*this < good) && (good < *this));
    }

    QString name;
    QString address;
    GoodType type;
    int kind;
    int buyPrice;
    int buyLevel;
    int sellPrice;
    int num;
};

const Good goodDefin[111][111] = {
    {
        Good(QString("empty"), QString("empty"), Seed, 0, 0, 0, 0),
        Good(QString("草莓"), QString("0"), Seed, 1, 10, 1, 20),
        Good(QString("玉米"), QString("1"), Seed, 2, 12, 1, 24),
        Good(QString("白萝卜"), QString("2"), Seed, 3, 15, 1, 30),
        Good(QString("胡萝卜"), QString("3"), Seed, 4, 20, 2, 40),
        Good(QString("地瓜"), QString("4"), Seed, 5, 22, 2, 44),
        Good(QString("茄子"), QString("5"), Seed, 6, 25, 2, 50),
        Good(QString("西红柿"), QString("6"), Seed, 7, 30, 3, 60),
        Good(QString("绿豆"), QString("7"), Seed, 8, 33, 3, 66),
        Good(QString("辣椒"), QString("8"), Seed, 9, 35, 3, 70),
        Good(QString("南瓜"), QString("9"), Seed, 10, 50, 4, 100),
        Good(QString("苹果"), QString("10"), Seed, 11, 60, 4, 120),
        Good(QString("西瓜"), QString("11"), Seed, 12, 70, 4, 140),
        Good(QString("香蕉"), QString("12"), Seed, 13, 100, 5, 200),
        Good(QString("桃子"), QString("13"), Seed, 14, 120, 5, 240),
        Good(QString("菠萝"), QString("14"), Seed, 15, 300, 6, 600)
    },
    {

    },
    {
        Good(QString("empty"), QString("empty"), Fertilize, 0, 0, 0, 0),
        Good(QString("初级化肥"), QString("1"), Fertilize, 1, 10, 1, 10),
        Good(QString("中极化肥"), QString("2"), Fertilize, 2, 20, 2, 20),
        Good(QString("高级化肥"), QString("3"), Fertilize, 3, 30, 3, 30)
    }
};

Good createGood(GoodType type, int kind, int num);

class BusinessDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BusinessDialog(Business business, Good good, QWidget *parent = 0);
    ~BusinessDialog();

protected:
    void paintEvent(QPaintEvent * event);

signals:
    void business_success(Good good, int num);

public slots:
    void checkNum();
    void send_infor();

private:
    QLineEdit * business_num;
    QPushButton * confirm;
    QPushButton * cancel;
    Business business;
    Good good;

    int price;
};

#endif // BUSINESSDIALOG_H
