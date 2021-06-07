/****************************************************************************
**
** Copyright (C) 2021 Robert Vetter.
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

#ifndef MSSWITCHERTHREAD_H
#define MSSWITCHERTHREAD_H

#include <QThread>

#include <iostream>
#include <array>
#include <vector>
#include <map>
#include <alsa/asoundlib.h>

#include "midiportidalsa.h"

#include "magicstomp.h"
#include "alsafunc.h"

class MSSwitcherThread : public QThread
{
    Q_OBJECT
public:
    MSSwitcherThread(QObject *parent=nullptr);
    ~MSSwitcherThread();

public slots:

signals:
    void programChanged(unsigned char val);
    void patchNameChanged(unsigned int id, QString patchName);
    void patchRequested(unsigned int id, int patchIndex);
    void msDisconnected(unsigned int id);

protected:
    void run();
private:
    snd_seq_t *handle;
    MidiClientPortId thisInPort;
    MidiClientPortId thisOutPort;

    struct MSDataState
    {
        int patchInRequest{-1};
        SysExDumpState dumpState{SysExDumpState::Idle};
        std::vector<unsigned char> data;
    };

    std::map<MidiClientPortId, MSDataState> msMap;

    int requestPatch(unsigned char index, const MidiClientPortId &srcPort, const MidiClientPortId &destPort);

    int sendPatchToTemp(unsigned char index, const MidiClientPortId &srcPort, const MidiClientPortId &destPort,
                        const unsigned char *patchCommonData, const unsigned char *patchEffectData);

    void scan();

    static bool isMagicstomp(const char *clientName, const char *portName);
    static unsigned char calcChecksum(const unsigned char *data, int dataLength);
};

#endif // MSSWITCHERTHREAD_H
