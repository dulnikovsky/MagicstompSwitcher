
#include <QApplication>
#include "mainwidget.h"
#include "msswitcherthread.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWidget mw;

    MSSwitcherThread mssThread;

    QObject::connect(&mssThread, SIGNAL(programChanged(unsigned char)), &mw, SLOT(setCurrentProgram(unsigned char)));

    mssThread.start();

    mw.showMaximized();

    int ret = app.exec();

    mssThread.terminate();
    mssThread.wait();
    return ret;
}
