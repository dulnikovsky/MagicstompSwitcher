/****************************************************************************
**
** Copyright (C) 2025 Robert Vetter.
**
** This file is part of the MagicstompSwitcher
**
** THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
** ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
** IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
** PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
**
** GNU General Public License Usage
** This file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version . The licenses are
** as published by the Free Software Foundation and appearing in the file LICENSE
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**/

#include "msswitcherapp.h"

#include "msswitcherthread.h"
#include "msswitchersettings.h"
#include <QTimer>

#ifdef WITH_SSD1306_DISPLAY
#include "ssd1306_display.h"
#include <linux/gpio.h>
#endif
#ifdef WITH_QT_GUI
#include <QKeyEvent>
#endif

MSSwitcherApp::MSSwitcherApp(int &argc, char **argv)
#ifdef WITH_QT_GUI
    : QApplication(argc, argv)
#else
    :QCoreApplication(argc, argv)
#endif
{
    setApplicationName(QStringLiteral("MagicstompSwitcher"));
    setOrganizationName(QStringLiteral("RobertVetter"));
    setOrganizationDomain(QStringLiteral("robertvetter.com"));
    setApplicationVersion(QStringLiteral("1.10"));

    MSSwitcherSettings settings;

    mssThread = new MSSwitcherThread(this);
    mssThread->setMidiChannel(settings.MidiChannel());
    mssThread->setMasterCCNumber(settings.MasterCCNumber());
    mssThread->setGainCCNumber(settings.GainCCNumber());
    mssThread->setEffectCCNumber(settings.EffectCCNumber());
    mssThread->setMidiThrough(settings.MidiThrough());

    connect(mssThread,SIGNAL(programChanged(uchar)),this,SIGNAL(programChanged(uchar)));
    connect(mssThread,SIGNAL(currentPatchChanged(uint,QString,bool)),this,SIGNAL(currentPatchChanged(uint,QString,bool)));
    connect(mssThread,SIGNAL(msDisconnected(uint)),this,SIGNAL(msDisconnected(uint)));

#ifdef WITH_SSD1306_DISPLAY
    ssd1306display = new SSD1306Display("/dev/i2c-0");

    connect(mssThread, SIGNAL(programChanged(unsigned char)), ssd1306display, SLOT(setCurrentProgram(unsigned char)));
    connect(mssThread, SIGNAL(currentPatchChanged(unsigned int, QString, bool)), ssd1306display, SLOT(onCurrentPatchChanged(unsigned int, QString, bool)));
    connect(mssThread, SIGNAL(msDisconnected(unsigned int)), ssd1306display, SLOT(onMsDisconnected(unsigned int)));
#endif

    mssThread->start();

#ifdef WITH_SSD1306_DISPLAY
    static const QList<int>offsetList({0, 2, 3});
    gpioHandlerThread = new GPIOHandlerThread(QStringLiteral("/dev/gpiochip0"), offsetList, this);
    connect(gpioHandlerThread, SIGNAL(gpioEvent(int, int)), this, SLOT(onGpioEvent(int, int)));
    gpioHandlerThread->start();
#endif
    f1KeyTimer = new QTimer(this);
    connect(f1KeyTimer, SIGNAL(timeout()), this, SLOT(onF1Timer()));
    connect(this, SIGNAL(aboutToQuit()), this, SLOT(isQuitting()));
}

void MSSwitcherApp::isQuitting()
{
    mssThread->exitEventLoop();
    mssThread->wait();
#ifdef WITH_SSD1306_DISPLAY
    gpioHandlerThread->finish();
    gpioHandlerThread->wait();
#endif
}

void MSSwitcherApp::onGpioEvent(int offset, int id)
{
#ifdef WITH_SSD1306_DISPLAY
    if(offset==0) { // F1 Key
        if(id==GPIO_V2_LINE_EVENT_RISING_EDGE && mssThread->isInSystexDumpState() == false) {
            f1KeyTimer->start(2000);
        } else if(id==GPIO_V2_LINE_EVENT_FALLING_EDGE) {
            if(! f1KeyTimer->isActive())
                return;
            f1KeyTimer->stop();
            ssd1306display->setInverted(false);
            if(f1TimerCounter==1 && mssThread->isInSystexDumpState() == false) {
                mssThread->exitEventLoop();
                mssThread->wait();
                mssThread->start();
            }
            f1TimerCounter = 0;
        }
    }
    else if(offset==2) { // F2 Key
        if(id==GPIO_V2_LINE_EVENT_FALLING_EDGE) {
            mssThread->switchPatchUp();
        }
    }
    else if(offset==3) { // F3 Key
        if(id==GPIO_V2_LINE_EVENT_FALLING_EDGE) {
            mssThread->switchPatchDown();
        }
    }
#endif
}

#ifdef WITH_QT_GUI
bool MSSwitcherApp::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::KeyPress) {
        QKeyEvent *keyPressEvent = static_cast<QKeyEvent *>(event);
        if(keyPressEvent->key()==Qt::Key_Control && mssThread->isInSystexDumpState() == false) {
            f1KeyTimer->start(2000);
            return true;
        }
    }
    else if(event->type() == QEvent::KeyRelease) {
        QKeyEvent *keyReleaseEvent = static_cast<QKeyEvent *>(event);
        if(keyReleaseEvent->key()==Qt::Key_Control) {
            f1KeyTimer->stop();
            if(f1TimerCounter==1 && mssThread->isInSystexDumpState() == false) {
                mssThread->exitEventLoop();
                mssThread->wait();
                mssThread->start();
            }
            f1TimerCounter = 0;
            return true;
        }
        if(keyReleaseEvent->key()==Qt::Key_Up) {
            mssThread->switchPatchUp();
            return true;
        }
        if(keyReleaseEvent->key()==Qt::Key_Down) {
            mssThread->switchPatchDown();
            return true;
        }
    }
    return QApplication::eventFilter(obj, event);
}
#endif

void MSSwitcherApp::onF1Timer()
{
    f1TimerCounter++;
#ifdef WITH_SSD1306_DISPLAY
    if(f1TimerCounter == 1) {
        ssd1306display->setInverted(true);
    } else {
        ssd1306display->setInverted(false);
    }
#endif
}

void MSSwitcherApp::setMidiChannel(unsigned int val)
{
    mssThread->setMidiChannel(val);
    MSSwitcherSettings settings;
    settings.setMidiChannel(val);
}
void MSSwitcherApp::setGainCCNumber(int val)
{
    mssThread->setGainCCNumber(val);
    MSSwitcherSettings settings;
    settings.setGainCCNumber(val);
}
void MSSwitcherApp::setMasterCCNumber(int val)
{
    mssThread->setMasterCCNumber(val);
    MSSwitcherSettings settings;
    settings.setMasterCCNumber(val);
}
void MSSwitcherApp::setEffectCCNumber(int val)
{
    mssThread->setEffectCCNumber(val);
    MSSwitcherSettings settings;
    settings.setEffectCCNumber(val);
}
void MSSwitcherApp::setMidiThrough(bool val)
{
    mssThread->setMidiThrough(val);
    MSSwitcherSettings settings;
    settings.setMidiThrough(val);
}

unsigned int MSSwitcherApp::MidiChannel() const
{
    return mssThread->MidiChannel();
}
int MSSwitcherApp::GainCCNumber() const
{
    return mssThread->GainCCNumber();
}
int MSSwitcherApp::MasterCCNumber() const
{
    return mssThread->MasterCCNumber();
}
int MSSwitcherApp::EffectCCNumber() const
{
    return mssThread->EffectCCNumber();
}
bool MSSwitcherApp::MidiThrough() const
{
    return mssThread->MidiThrough();
}
