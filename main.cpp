#include "mainview.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);
    QApplication a(argc, argv);

    MainView w;
    w.start();

    return a.exec();
}
