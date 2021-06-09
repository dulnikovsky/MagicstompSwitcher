
#include <QApplication>
#include "mainwidget.h"
#include "msswitcherthread.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWidget mw;

    MSSwitcherThread mssThread;

    QObject::connect(&mssThread, SIGNAL(programChanged(unsigned char)), &mw, SLOT(setCurrentProgram(unsigned char)));
    QObject::connect(&mssThread, SIGNAL(currentPatchChanged(unsigned int, QString, bool)),
                     &mw, SLOT(onCurrentPatchChanged(unsigned int, QString, bool)));


    QObject::connect(&mssThread, SIGNAL(msDisconnected(unsigned int)), &mw, SLOT(onMsDisconnected(unsigned int)));

    mssThread.start();

    mw.showMaximized();

    int ret = app.exec();

    mssThread.terminate();
    mssThread.wait();
    return ret;
}
