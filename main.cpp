
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

int main(int argc, char *argv[])
{
    MSSwitcherApp app(argc, argv);

 #ifdef WITH_QT_GUI
    MainWidget mw;

    QObject::connect(&app, SIGNAL(programChanged(unsigned char)), &mw, SLOT(setCurrentProgram(unsigned char)));
    QObject::connect(&app, SIGNAL(currentPatchChanged(unsigned int, QString, bool)),
                     &mw, SLOT(onCurrentPatchChanged(unsigned int, QString, bool)));

    QObject::connect(&app, SIGNAL(msDisconnected(unsigned int)), &mw, SLOT(onMsDisconnected(unsigned int)));

    QObject::connect(&mw, SIGNAL(midiChannelChanged(unsigned int)), &app, SLOT(setMidiChannel(unsigned int)));
    QObject::connect(&mw, SIGNAL(gainCCNumberChanged(int)), &app, SLOT(setGainCCNumber(int)));
    QObject::connect(&mw, SIGNAL(masterCCNumberChanged(int)), &app, SLOT(setMasterCCNumber(int)));
    QObject::connect(&mw, SIGNAL(effectLevelCCNumberChanged(int)), &app, SLOT(setEffectCCNumber(int)));
    QObject::connect(&mw, SIGNAL(midiThroughChanged(bool)), &app, SLOT(setMidiThrough(bool)));

    if(app.platformName() == QStringLiteral("linuxfb"))
    {
        mw.showFullScreen();
    }
    else
    {
        mw.resize(480, 320);
        mw.show();
    }
    mw.installEventFilter(&app);
#endif
    int ret = app.exec();
    return ret;
}
