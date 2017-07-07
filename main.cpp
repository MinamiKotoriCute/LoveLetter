#include "mainwindow.h"
#include <QApplication>

#include "loveletterwidget.h"

#include "agent.h"

#include "maptreewidget.h"
#include "easywebcontrol.h"

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
    //MapTreeWidget m;
    //m.setData(loadJson());
    //m.show();

    return a.exec();
}
