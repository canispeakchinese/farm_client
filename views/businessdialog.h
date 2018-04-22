#ifndef BUSINESSDIALOG_H
#define BUSINESSDIALOG_H

#include "mainview.h"
#include <QDialog>
#include <QPushButton>
#include <QPaintEvent>

class QLineEdit;
class QGraphicsView;

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
    void business_success(Good good);

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
