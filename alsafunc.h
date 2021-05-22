
#ifndef ALSAFUNC_H
#define ALSAFUNC_H


#include <alsa/asoundlib.h>
#include "midiportidalsa.h"

int subscribePort(snd_seq_t *handle, const MidiClientPortId &senderPortId, const MidiClientPortId &destinationPortId);

int unSubscribePort(snd_seq_t *handle, const MidiClientPortId &senderPortId, const MidiClientPortId &destinationPortId);

#endif
