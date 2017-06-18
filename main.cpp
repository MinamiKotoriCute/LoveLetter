#include "mainwindow.h"
#include <QApplication>

#include "loveletterwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    LoveLetterWidget lw;
    lw.show();

    return a.exec();
}
