#include "mainwindow.h"
#include <QApplication>

#include "maptreewidget.h"
#include "commonfunction.h"

#include "loveletterwidget.h"
#include "agent.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /// defaulf, no change
    MainWindow w;
    w.show();

    /// player game window
    //LoveLetterWidget lw;
    //lw.show();

    /// learning agent
    //Agent ag;

    /// show agent result
    MapTreeWidget m;
    m.setData(loadJson());
    m.show();

    return a.exec();
}
