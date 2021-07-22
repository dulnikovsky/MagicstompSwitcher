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
    void setMidiChannel(unsigned int val) { midiChannel=val;}
    void setGainCCNumber(int val) { gainCCNumber=val;}
    void setMasterCCNumber(int val) { masterCCNumber=val;}
    void setEffectCCNumber(int val) { effectLevelCCNumber=val;}

public:
    unsigned int MidiChannel() const { return midiChannel;}
    int GainCCNumber() const { return gainCCNumber; }
    int MasterCCNumber() const { return masterCCNumber; }
    int EffectCCNumber() const { return effectLevelCCNumber; }

signals:
    void programChanged(unsigned char val);
    void currentPatchChanged(unsigned int id, QString patchName, bool isRequesting);
    void msDisconnected(unsigned int id);

protected:
    void run();
private:
    snd_seq_t *handle;
    MidiClientPortId thisInPort;
    MidiClientPortId thisOutPort;

    unsigned int midiChannel{0};
    int gainCCNumber{-1};
    int masterCCNumber{-1};
    int effectLevelCCNumber{-1};

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

    void sendTempEffectParameter(const MidiClientPortId &srcPort, const MidiClientPortId &destPort, unsigned char offset, const std::vector<unsigned char> &data);

    void scan();

    static bool isMagicstomp(const char *clientName, const char *portName);
    static unsigned char calcChecksum(const unsigned char *data, int dataLength);
};

#endif // MSSWITCHERTHREAD_H
