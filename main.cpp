
#ifdef WITH_QT_GUI
#include <QApplication>
#include "mainwidget.h"
#else
#include <QCoreApplication>
#endif

#ifdef WITH_SSD1306_DISPLAY
#include "ssd1306_display.h"
#endif

#include "msswitcherthread.h"
#include "msswitchersettings.h"

const MSSwitcherThread *switcherThread;

int main(int argc, char *argv[])
{

#ifdef WITH_QT_GUI
    QApplication app(argc, argv);
#else
    QCoreApplication app(argc, argv);
#endif

    app.setApplicationName(QStringLiteral("MagicstompSwitcher"));
    app.setOrganizationName(QStringLiteral("RobertVetter"));
    app.setOrganizationDomain(QStringLiteral("robertvetter.com"));
    app.setApplicationVersion(QStringLiteral("1.00"));

    MSSwitcherSettings settings;

    MSSwitcherThread mssThread;
    switcherThread = &mssThread;
    mssThread.setMidiChannel(settings.MidiChannel());
    mssThread.setMasterCCNumber(settings.MasterCCNumber());
    mssThread.setGainCCNumber(settings.GainCCNumber());
    mssThread.setEffectCCNumber(settings.EffectCCNumber());
    mssThread.setMidiThrough(settings.MidiThrough());

 #ifdef WITH_QT_GUI
    MainWidget mw;

    QObject::connect(&mssThread, SIGNAL(programChanged(unsigned char)), &mw, SLOT(setCurrentProgram(unsigned char)));
    QObject::connect(&mssThread, SIGNAL(currentPatchChanged(unsigned int, QString, bool)),
                     &mw, SLOT(onCurrentPatchChanged(unsigned int, QString, bool)));

    QObject::connect(&mssThread, SIGNAL(msDisconnected(unsigned int)), &mw, SLOT(onMsDisconnected(unsigned int)));

    QObject::connect(&mw, SIGNAL(midiChannelChanged(unsigned int)), &mssThread, SLOT(setMidiChannel(unsigned int)));
    QObject::connect(&mw, SIGNAL(gainCCNumberChanged(int)), &mssThread, SLOT(setGainCCNumber(int)));
    QObject::connect(&mw, SIGNAL(masterCCNumberChanged(int)), &mssThread, SLOT(setMasterCCNumber(int)));
    QObject::connect(&mw, SIGNAL(effectLevelCCNumberChanged(int)), &mssThread, SLOT(setEffectCCNumber(int)));
    QObject::connect(&mw, SIGNAL(midiThroughChanged(bool)), &mssThread, SLOT(setMidiThrough(bool)));

    QObject::connect(&mw, SIGNAL(midiChannelChanged(unsigned int)), &settings, SLOT(setMidiChannel(unsigned int)));
    QObject::connect(&mw, SIGNAL(gainCCNumberChanged(int)), &settings, SLOT(setGainCCNumber(int)));
    QObject::connect(&mw, SIGNAL(masterCCNumberChanged(int)), &settings, SLOT(setMasterCCNumber(int)));
    QObject::connect(&mw, SIGNAL(effectLevelCCNumberChanged(int)), &settings, SLOT(setEffectCCNumber(int)));
    QObject::connect(&mw, SIGNAL(midiThroughChanged(bool)), &settings, SLOT(setMidiThrough(bool)));
#endif

#ifdef WITH_SSD1306_DISPLAY
    SSD1306Display ssd1306display("/dev/i2c-0");

    QObject::connect(&mssThread, SIGNAL(programChanged(unsigned char)), &ssd1306display, SLOT(setCurrentProgram(unsigned char)));
    QObject::connect(&mssThread, SIGNAL(currentPatchChanged(unsigned int, QString, bool)),
                     &ssd1306display, SLOT(onCurrentPatchChanged(unsigned int, QString, bool)));
#endif

    mssThread.start();

 #ifdef WITH_QT_GUI
    if(app.platformName() == QStringLiteral("linuxfb"))
    {
        mw.showFullScreen();
    }
    else
    {
        mw.resize(480, 320);
        mw.show();
    }
#endif
    int ret = app.exec();

    mssThread.terminate();
    mssThread.wait();
    return ret;
}
