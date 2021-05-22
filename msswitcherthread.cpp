/****************************************************************************
**
** Copyright (C) 2021 Robert Vetter.
**
** This file is part of the MagicstompFrenzy - an editor for Yamaha Magicstomp
** effect processor
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

#include "msswitcherthread.h"

using namespace std;

static constexpr int ub99SysExHeaderSize = 8;
static const unsigned char ub99SysExHeader[ub99SysExHeaderSize] = { 0xF0, 0x43, 0x7D, 0x30, 0x55, 0x42, 0x39, 0x39 };

static const char *appName = "MagicstompSwitcher";

MSSwitcherThread::MSSwitcherThread(QObject *parent)
    :QThread(parent)
{
    snd_seq_open(&handle, "default", SND_SEQ_OPEN_DUPLEX, 0);

    snd_seq_set_client_name(handle, appName);

    unsigned char inPort = snd_seq_create_simple_port(handle, "IN",
                    SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE, SND_SEQ_PORT_TYPE_MIDI_GENERIC | SND_SEQ_PORT_TYPE_APPLICATION);
    unsigned char outPort = snd_seq_create_simple_port(handle, "OUT",
                    SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ, SND_SEQ_PORT_TYPE_MIDI_GENERIC );

    thisInPort = MidiClientPortId(snd_seq_client_id(handle), inPort);
    thisOutPort = MidiClientPortId(snd_seq_client_id(handle), outPort);

    subscribePort(handle, MidiClientPortId(SND_SEQ_CLIENT_SYSTEM, SND_SEQ_PORT_SYSTEM_ANNOUNCE), thisInPort);
}

MSSwitcherThread::~MSSwitcherThread()
{
    snd_seq_close(handle);
}

int MSSwitcherThread::requestPatch(unsigned char index, const MidiClientPortId &srcPort, const MidiClientPortId &destPort)
{
    array<unsigned char, 10> request = {0xF0, 0x43, 0x7D, 0x50, 0x55, 0x42, 0x30, 0x01, 0x00, 0xF7};
    request[8] = index;

    snd_seq_event_t sendev;
    snd_seq_ev_clear(&sendev);
    snd_seq_ev_set_source(&sendev, srcPort.portId());
    snd_seq_ev_set_dest(&sendev, destPort.clientId(), destPort.portId());
    snd_seq_ev_set_direct(&sendev);

    snd_seq_ev_set_variable(&sendev, request.size(), (void *) &request.at(0));
    sendev.type=SND_SEQ_EVENT_SYSEX;

    int ret;
    ret = snd_seq_event_output(handle, &sendev);
    if( ret <= 0)
        return ret;
    ret = snd_seq_drain_output(handle);
    return ret;
}

int MSSwitcherThread::sendPatchToTemp(unsigned char index, const MidiClientPortId &srcPort, const MidiClientPortId &destPort,
                    const unsigned char *patchCommonData, const unsigned char *patchEffectData)
{
    vector<unsigned char> dataVector;
    snd_seq_event_t sendev;
    int ret;

    dataVector.insert(dataVector.end(), ub99SysExHeader, ub99SysExHeader + ub99SysExHeaderSize);
    dataVector.push_back(0x00);
    dataVector.push_back(0x00);
    dataVector.push_back(0x30);
    dataVector.push_back(0x03);
    dataVector.push_back(index);
    dataVector.push_back( calcChecksum( &(*dataVector.cbegin()) + ub99SysExHeaderSize, dataVector.size() - ub99SysExHeaderSize));
    dataVector.push_back(0xF7);

    snd_seq_ev_clear(&sendev);
    snd_seq_ev_set_source(&sendev, srcPort.portId());
    snd_seq_ev_set_dest(&sendev, destPort.clientId(), destPort.portId());
    snd_seq_ev_set_direct(&sendev);
    snd_seq_ev_set_variable(&sendev, dataVector.size(), (void *) &(*dataVector.cbegin()));
    sendev.type=SND_SEQ_EVENT_SYSEX;
    ret = snd_seq_event_output(handle, &sendev);
    if( ret <= 0)
        return ret;

    dataVector.clear();
    dataVector.insert(dataVector.end(), ub99SysExHeader, ub99SysExHeader + ub99SysExHeaderSize);
    dataVector.push_back(0x00);
    dataVector.push_back(PatchCommonLength);
    dataVector.push_back(0x20);
    dataVector.push_back(0x00);
    dataVector.push_back(0x00);
    dataVector.insert(dataVector.end(), patchCommonData, patchCommonData + PatchCommonLength);
    dataVector.push_back( calcChecksum( &(*dataVector.cbegin()) + ub99SysExHeaderSize, dataVector.size() - ub99SysExHeaderSize));
    dataVector.push_back(0xF7);

    snd_seq_ev_clear(&sendev);
    snd_seq_ev_set_source(&sendev, srcPort.portId());
    snd_seq_ev_set_dest(&sendev, destPort.clientId(), destPort.portId());
    snd_seq_ev_set_direct(&sendev);
    snd_seq_ev_set_variable(&sendev, dataVector.size(), (void *) &(*dataVector.cbegin()));
    sendev.type=SND_SEQ_EVENT_SYSEX;
    ret = snd_seq_event_output(handle, &sendev);
    if( ret <= 0)
        return ret;

    dataVector.clear();
    dataVector.insert(dataVector.end(), ub99SysExHeader, ub99SysExHeader + ub99SysExHeaderSize);
    dataVector.push_back(0x00);
    dataVector.push_back(PatchEffectLength);
    dataVector.push_back(0x20);
    dataVector.push_back(0x01);
    dataVector.push_back(0x00);
    dataVector.insert(dataVector.end(), patchEffectData, patchEffectData + PatchEffectLength);
    dataVector.push_back( calcChecksum( &(*dataVector.cbegin()) + ub99SysExHeaderSize, dataVector.size() - ub99SysExHeaderSize));
    dataVector.push_back(0xF7);

    snd_seq_ev_clear(&sendev);
    snd_seq_ev_set_source(&sendev, srcPort.portId());
    snd_seq_ev_set_dest(&sendev, destPort.clientId(), destPort.portId());
    snd_seq_ev_set_direct(&sendev);
    snd_seq_ev_set_variable(&sendev, dataVector.size(), (void *) &(*dataVector.cbegin()));
    sendev.type=SND_SEQ_EVENT_SYSEX;
    ret = snd_seq_event_output(handle, &sendev);
    if( ret <= 0)
        return ret;

    dataVector.clear();
    dataVector.insert(dataVector.end(), ub99SysExHeader, ub99SysExHeader + ub99SysExHeaderSize);
    dataVector.push_back(0x00);
    dataVector.push_back(0x00);
    dataVector.push_back(0x30);
    dataVector.push_back(0x13);
    dataVector.push_back(index);
    dataVector.push_back( calcChecksum( &(*dataVector.cbegin()) + ub99SysExHeaderSize, dataVector.size() - ub99SysExHeaderSize));
    dataVector.push_back(0xF7);

    snd_seq_ev_clear(&sendev);
    snd_seq_ev_set_source(&sendev, srcPort.portId());
    snd_seq_ev_set_dest(&sendev, destPort.clientId(), destPort.portId());
    snd_seq_ev_set_direct(&sendev);
    snd_seq_ev_set_variable(&sendev, dataVector.size(), (void *) &(*dataVector.cbegin()));
    sendev.type=SND_SEQ_EVENT_SYSEX;
    ret = snd_seq_event_output(handle, &sendev);
    if( ret <= 0)
        return ret;

    ret = snd_seq_drain_output(handle);
    return ret;
}

void MSSwitcherThread::scan()
{
    snd_seq_client_info_t *cinfo;
    snd_seq_port_info_t *pinfo;

    snd_seq_client_info_alloca(&cinfo);
    snd_seq_port_info_alloca(&pinfo);
    snd_seq_client_info_set_client(cinfo, -1);
    while (snd_seq_query_next_client(handle, cinfo) >= 0)
    {
        int clientId = snd_seq_client_info_get_client(cinfo);
        if( (clientId == SND_SEQ_CLIENT_SYSTEM) || (clientId == snd_seq_client_id(handle)))
            continue;

        snd_seq_port_info_set_client(pinfo, clientId);
        snd_seq_port_info_set_port(pinfo, -1);
        while (snd_seq_query_next_port(handle, pinfo) >= 0)
        {
            MidiClientPortId mcpid(clientId, snd_seq_port_info_get_port(pinfo));
            unsigned int cap = snd_seq_port_info_get_capability(pinfo);

            if( isMagicstomp(snd_seq_client_info_get_name(cinfo), snd_seq_port_info_get_name(pinfo)))
            {
                msMap.insert( {mcpid, MSDataState()} );
                subscribePort(handle, thisOutPort, mcpid);

                cout << "Magicstomp found[" << static_cast<unsigned int>(mcpid.clientId())
                     << "," << static_cast<unsigned int>(mcpid.portId()) << "]" << endl;
            }
            else if((snd_seq_port_info_get_type(pinfo) & SND_SEQ_PORT_TYPE_HARDWARE) == SND_SEQ_PORT_TYPE_HARDWARE &&
                    cap & (SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ))
            {
                subscribePort( handle, mcpid, thisInPort);
                cout << "Hardware MIDI IN device found[" << static_cast<unsigned int>(mcpid.clientId())
                     << "," << static_cast<unsigned int>(mcpid.portId()) << "]" << endl;
            }

        }
    }
}

void MSSwitcherThread::run()
{
    unsigned char currentProgram = 0;
    emit programChanged(currentProgram);
    snd_seq_event_t *ev;

    msMap.clear();
    scan();

    map<MidiClientPortId, vector<unsigned char>> sysExBufferMap;
    for (auto& it: msMap)
    {
        sysExBufferMap.insert({it.first, vector<unsigned char>()});

        subscribePort(handle, it.first, thisInPort);
        it.second.dumpState = SysExDumpState::ExpectingStart;
        requestPatch(++(it.second.patchInRequest), thisOutPort, it.first);
    }

    // IMPORTANT snd_seq_event_input blocks even after snd_seq_close has been execuded.
    // It will be  neccessary for this application to send an SND_SEQ_EVENT_CLIENT_EXIT to himself before terminating
    while (snd_seq_event_input(handle, &ev) >= 0)
    {
        if(ev->type==SND_SEQ_EVENT_SYSEX)
        {
            MidiClientPortId currentID(MidiClientPortId(ev->source.client, ev->source.port));

            auto sysExBufferIter = sysExBufferMap.find(currentID);
            if(sysExBufferIter == sysExBufferMap.end())
                continue;

            auto &sysExVector = sysExBufferIter->second;

            if(static_cast<unsigned char *>(ev->data.ext.ptr)[0] == 0xF0)
                sysExVector.clear();

            if(ev->data.ext.len>256 || sysExVector.size()>256)
                continue; // Sanity check

            auto msMapIter = msMap.find(currentID);
            if(msMapIter == msMap.end())
                continue;

            auto &msDataState = msMapIter->second;

            sysExVector.insert(sysExVector.end(), static_cast<unsigned char *>(ev->data.ext.ptr), static_cast<unsigned char *>(ev->data.ext.ptr) + ev->data.ext.len);

            if( (! sysExVector.empty()) && sysExVector.at(0) == 0xF0 && sysExVector.at(sysExVector.size()-1) == 0xF7)
            {
                if(sysExVector.size() >= 13 && equal(sysExVector.cbegin(), sysExVector.cbegin()+ub99SysExHeaderSize, ub99SysExHeader))
                {
                    unsigned char checkSum = calcChecksum( & sysExVector.at(ub99SysExHeaderSize), sysExVector.size()-ub99SysExHeaderSize-2);
                    if(checkSum == sysExVector.at(sysExVector.size()-2))
                    {
                        if( sysExVector.at(8)==0x00 && sysExVector.at(9)==0x00)
                        {
                            //patch dump start message.
                            if( sysExVector.at(10)==0x30 && sysExVector.at(11)==0x01 && msDataState.patchInRequest == sysExVector.at(12) && msDataState.dumpState == SysExDumpState::ExpectingStart)
                            {
                                msDataState.dumpState = SysExDumpState::ExpectingCommonData;
                            }
                            //patch dump end message.
                            else if( sysExVector.at(10)==0x30 && sysExVector.at(11)==0x11 && msDataState.patchInRequest == sysExVector.at(12) && msDataState.dumpState == SysExDumpState::ExpectingEnd)
                            {
                                msDataState.patchInRequest++;
                                if(msDataState.patchInRequest >= numOfPatches)
                                {
                                    msDataState.dumpState = SysExDumpState::Idle;
                                    msDataState.patchInRequest = -1;
                                    unSubscribePort(handle, msMapIter->first, thisInPort);
                                }
                                else
                                {
                                    msDataState.dumpState = SysExDumpState::ExpectingStart;
                                    requestPatch(msDataState.patchInRequest, thisOutPort, currentID);
                                }
                            }
                        }
                        else if( sysExVector.at(8)==0x00 &&  sysExVector.at(9)!=0x00)
                        {
                            unsigned char length = sysExVector.at(9);
                            if(sysExVector.at(10)==0x20)
                            {
                                auto iter = msMap.find(MidiClientPortId(ev->source.client, ev->source.port));
                                if(sysExVector.at(11)==0x00 && sysExVector.at(12)==0x00 && length==PatchCommonLength && msDataState.dumpState==SysExDumpState::ExpectingCommonData)
                                { // Patch common data;
                                    if( iter != msMap.end())
                                        iter->second.data.insert(iter->second.data.end(), &sysExVector.at(13), &sysExVector.at(13)+PatchCommonLength);
                                    msDataState.dumpState = SysExDumpState::ExpectingEffectData;
                                }
                                else if(sysExVector.at(11)==0x01 && sysExVector.at(12)==0x00 && length==PatchEffectLength && msDataState.dumpState==SysExDumpState::ExpectingEffectData)
                                { // Patch effect data;
                                    if( iter != msMap.end())
                                        iter->second.data.insert(iter->second.data.end(), &sysExVector.at(13), &sysExVector.at(13)+PatchEffectLength);
                                    msDataState.dumpState = SysExDumpState::ExpectingEnd;
                                }
                            }
                        }
                    }
                }
                sysExVector.clear();
            }

        }
        else if(ev->type==SND_SEQ_EVENT_PGMCHANGE)
        {
            // TODO if( ev->data.raw8.d[0] & 0x0F == channel)

            if(ev->data.raw8.d[8] < numOfPatches)
            {
                currentProgram = ev->data.raw8.d[8];
                emit programChanged(currentProgram);

                for (auto& it: msMap)
                {
                    if( it.second.data.size() != numOfPatches*PatchTotalLength)
                        continue; // Size does not match, means something went wrong

                    if( it.second.dumpState != SysExDumpState::Idle)
                        continue;

                    sendPatchToTemp(currentProgram, thisOutPort, it.first,
                                    &(*(it.second.data.cbegin()+PatchTotalLength*currentProgram)),
                                    &(*(it.second.data.cbegin()+PatchTotalLength*currentProgram)) + PatchCommonLength);
                }
            }
        }
        else if(ev->type==SND_SEQ_EVENT_PORT_START)
        {
            snd_seq_client_info_t *cinfo;
            snd_seq_port_info_t *pinfo;

            snd_seq_client_info_alloca(&cinfo);
            snd_seq_port_info_alloca(&pinfo);

            snd_seq_get_any_client_info(handle, ev->data.addr.client, cinfo);
            snd_seq_get_any_port_info(handle, ev->data.addr.client, ev->data.addr.port, pinfo);
            unsigned int cap = snd_seq_port_info_get_capability(pinfo);
            MidiClientPortId mscpid(ev->data.addr.client, ev->data.addr.port);

            if(isMagicstomp(snd_seq_client_info_get_name(cinfo), snd_seq_port_info_get_name(pinfo)))
            {
                auto findIter = msMap.find(mscpid);
                if(findIter == msMap.end())
                {
                    sysExBufferMap.insert({mscpid, vector<unsigned char>()});

                    subscribePort(handle, thisOutPort, mscpid);
                    subscribePort(handle, mscpid, thisInPort);
                    pair<map<MidiClientPortId, MSDataState>::iterator,bool> ret;
                    ret = msMap.insert({mscpid, MSDataState()} );
                    ret.first->second.dumpState = SysExDumpState::ExpectingStart;
                    this_thread::sleep_for(std::chrono::milliseconds(1000)); // Wait 1s until MS gets ready after power on
                    requestPatch(++(ret.first->second.patchInRequest), thisOutPort, ret.first->first);

                    cout << "Magicstomp connected[" << static_cast<unsigned int>(ev->data.addr.client)
                         << "," << static_cast<unsigned int>(ev->data.addr.port) << "]" << endl;
                }
            }
            else if((snd_seq_port_info_get_type(pinfo) & SND_SEQ_PORT_TYPE_HARDWARE) == SND_SEQ_PORT_TYPE_HARDWARE &&
                    cap & (SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ))
            {
                subscribePort( handle, mscpid, thisInPort);
                cout << "Hardware MIDI IN device connected[" << static_cast<unsigned int>(ev->data.addr.client)
                     << "," << static_cast<unsigned int>(ev->data.addr.port) << "]" << endl;
            }
        }
        else if(ev->type==SND_SEQ_EVENT_PORT_EXIT)
        {
            MidiClientPortId mscpid(ev->data.addr.client, ev->data.addr.port);

            if(msMap.erase(mscpid) == 1)
            {
                sysExBufferMap.erase(mscpid);
                cout << "Magicstomp disconnected[" << static_cast<unsigned int>(ev->data.addr.client)
                     << "," << static_cast<unsigned int>(ev->data.addr.port) << "]" << endl;
            }
        }

        //cout << "MIDI Event. Type = " << static_cast<int>(ev->type) << " Patch idx=" << patchInRequest << endl;
        snd_seq_free_event(ev);
    }

}

bool MSSwitcherThread::isMagicstomp(const char *clientName, const char *portName)
{
    const char *ub99ClientName = "UB99";
    const char *ub99PortName = "UB99 MIDI 1";

    return(strncmp(portName, ub99PortName, 11) == 0 && strncmp(clientName, ub99ClientName, 4) == 0);
}

unsigned char MSSwitcherThread::calcChecksum(const unsigned char *data, int dataLength)
{
    char checkSum = 0;
    for (int i = 0; i < dataLength; ++i)
    {
        checkSum += *data++;
    }
    return ((-checkSum) & 0x7f);
}
