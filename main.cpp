#include "mainwindow.h"
#include <QApplication>

#include "loveletterwidget.h"

#include "agent.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //LoveLetterWidget lw;
    //lw.show();

    Agent ag;

    return a.exec();
}
