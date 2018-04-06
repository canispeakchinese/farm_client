#include "businessdialog.h"
#include <QPainter>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QLineEdit>
#include <QRegExp>
#include <QValidator>

Good createGood(GoodType type, int kind, int num)
{
    if(type == Fruit)
    {
        Good aim(goodDefin[Seed][kind]);
        aim.type = Fruit;
        aim.num = num;
        return aim;
    }
    else
    {
        Good aim(goodDefin[type][kind]);
        aim.num = num;
        return aim;
    }
}

BusinessDialog::BusinessDialog(Business business, Good good, QWidget *parent) :
    QDialog(parent), business(business), good(good)
{
    setMinimumSize(400, 300);
    setMaximumSize(400, 300);
    setWindowFlags(Qt::FramelessWindowHint);

    business_num = new QLineEdit(this);
    QRegExp regx("[0-9]+$");
    QValidator *validator = new QRegExpValidator(regx, business_num);
    business_num->setValidator( validator );
    connect(business_num,SIGNAL(textChanged(QString)),this,SLOT(checkNum()));
    business_num->resize(50,0);
    business_num->setGeometry(240, 150, 70, 20);

    confirm = new QPushButton(this);
    if(business == Sell)
    {
        confirm->setText("卖出");
        if(good.type == Fruit)
            price = good.sellPrice;
        else if(good.type == Seed || good.type == Fertilize)
            price = good.buyPrice*0.8;
    }
    else
    {
        confirm->setText("买入");
        if(good.type == Seed || good.type == Fertilize)
            price = good.buyPrice;
    }
    cancel = new QPushButton("取消", this);
    confirm->setGeometry(130, 260, 50, 20);
    cancel->setGeometry(220, 260, 50, 20);

    connect(cancel,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(confirm,SIGNAL(clicked(bool)),this,SLOT(send_infor()));
}

void BusinessDialog::checkNum()
{
    if(business_num->text().toInt() > good.num)
        business_num->setText(QString::number(good.num));
    update(QRegion(200, 180, 150, 20));
}

void BusinessDialog::send_infor()
{
    emit business_success(good, business_num->text().toInt());
    close();
}

void BusinessDialog::paintEvent(QPaintEvent *event)
{
    QPainter * painter = new QPainter();
    painter->begin(this);

    QFont font("楷体", 30, QFont::Bold, false);//设置字体的类型，大小，加粗，斜体
    font.setCapitalization(QFont::SmallCaps);//设置大小写
    font.setLetterSpacing(QFont::AbsoluteSpacing,5);//设置间距
    painter->setFont(font);//添加字体
    QRectF ff(200,30,150,60);
    if(good.type == Seed || good.type == Fruit)
        painter->drawPixmap(50, 60, 100, 100, QPixmap(QString("%1/seed.png").arg(good.address)));
    else
        painter->drawPixmap(50, 60, 100, 100, QPixmap(good.address));
    painter->drawText(ff,Qt::AlignCenter,good.name);
    font.setPointSize(10);
    painter->setFont(font);
    painter->drawText(QRect(200, 110, 150, 20), Qt::AlignCenter, QString("单个价格:%1").arg(price));
    painter->drawText(QRect(200, 180, 150, 20), Qt::AlignCenter, QString("交易金额:%1").arg(price*business_num->text().toInt()));
    painter->end();
    QDialog::paintEvent(event);
}

BusinessDialog::~BusinessDialog()
{
}
