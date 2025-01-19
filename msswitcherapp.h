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

#ifndef MSSWITCHERAPP_H
#define MSSWITCHERAPP_H

#ifdef WITH_QT_GUI
#include <QApplication>
#include "mainwidget.h"
#else
#include <QCoreApplication>
#endif

#include <QElapsedTimer>
class MSSwitcherThread;
class SSD1306Display;

#ifdef WITH_SSD1306_DISPLAY
#include "gpiohandlerthread.h"
#endif

#ifdef WITH_QT_GUI
class MSSwitcherApp : public QApplication
#else
class MSSwitcherApp : public QCoreApplication
#endif
{
    Q_OBJECT
public:
    MSSwitcherApp(int &argc, char **argv);

public slots:
    void setMidiChannel(unsigned int val);
    void setGainCCNumber(int val);
    void setMasterCCNumber(int val);
    void setEffectCCNumber(int val);
    void setMidiThrough(bool val);

public:
    unsigned int MidiChannel() const;
    int GainCCNumber() const;
    int MasterCCNumber() const;
    int EffectCCNumber() const;
    bool MidiThrough() const;

#ifdef WITH_QT_GUI
protected:
    bool eventFilter(QObject *obj, QEvent *ev) override;
#endif

signals:
    void programChanged(unsigned char val);
    void currentPatchChanged(unsigned int id, QString patchName, bool isRequesting);
    void msDisconnected(unsigned int id);

private slots:
    void onGpioEvent(int offset, int id);
    void isQuitting();
private:
    MSSwitcherThread *mssThread;
#ifdef WITH_SSD1306_DISPLAY
    SSD1306Display *ssd1306display;
    GPIOHandlerThread *gpioHandlerThread;
#endif
    QElapsedTimer f1KeyTimer;
};

#endif
