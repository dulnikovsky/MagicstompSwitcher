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

#ifdef WITH_SSD1306_DISPLAY
#include "ssd1306_display.h"
#include <linux/gpio.h>
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
    connect(this, SIGNAL(aboutToQuit()), this, SLOT(isQuitting()));
}

void MSSwitcherApp::isQuitting()
{
    mssThread->terminate();
    mssThread->wait();
#ifdef WITH_SSD1306_DISPLAY
    gpioHandlerThread->finish();
    gpioHandlerThread->wait();
#endif
}

void MSSwitcherApp::onGpioEvent(int offset, int id)
{
    if(offset==0) { // F1 Key

    }
    else if(offset==2) { // F2 Key

    }
    else if(offset==3) { // F3 Key

    }
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
