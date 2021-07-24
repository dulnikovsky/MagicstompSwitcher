
#include <QApplication>
#include "mainwidget.h"
#include "msswitcherthread.h"

const MSSwitcherThread *switcherThread;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("MagicstompSwitcher"));
    app.setApplicationVersion(QStringLiteral("1.00"));

    MainWidget mw;

    MSSwitcherThread mssThread;
    switcherThread = &mssThread;
    mssThread.setMasterCCNumber(7);
    mssThread.setGainCCNumber(16);

    QObject::connect(&mssThread, SIGNAL(programChanged(unsigned char)), &mw, SLOT(setCurrentProgram(unsigned char)));
    QObject::connect(&mssThread, SIGNAL(currentPatchChanged(unsigned int, QString, bool)),
                     &mw, SLOT(onCurrentPatchChanged(unsigned int, QString, bool)));


    QObject::connect(&mssThread, SIGNAL(msDisconnected(unsigned int)), &mw, SLOT(onMsDisconnected(unsigned int)));


    QObject::connect(&mw, SIGNAL(midiChannelChanged(unsigned int)), &mssThread, SLOT(setMidiChannel(unsigned int)));
    QObject::connect(&mw, SIGNAL(gainCCNumberChanged(int)), &mssThread, SLOT(setGainCCNumber(int)));
    QObject::connect(&mw, SIGNAL(masterCCNumberChanged(int)), &mssThread, SLOT(setMasterCCNumber(int)));
    QObject::connect(&mw, SIGNAL(effectLevelCCNumberChanged(int)), &mssThread, SLOT(setEffectCCNumber(int)));
    QObject::connect(&mw, SIGNAL(midiThroughChanged(bool)), &mssThread, SLOT(setMidiThrough(bool)));

    mssThread.start();

    if(app.platformName() == QStringLiteral("linuxfb"))
    {
        mw.showFullScreen();
    }
    else
    {
        mw.resize(480, 320);
        mw.show();
    }

    int ret = app.exec();

    mssThread.terminate();
    mssThread.wait();
    return ret;
}
