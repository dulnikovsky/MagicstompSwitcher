
#include "alsafunc.h"

int subscribePort(snd_seq_t *handle, const MidiClientPortId &senderPortId, const MidiClientPortId &destinationPortId)
{
    snd_seq_addr_t sender, dest;
    snd_seq_port_subscribe_t* subs;
    snd_seq_port_subscribe_alloca(&subs);

    sender.client = senderPortId.clientId();
    sender.port = senderPortId.portId();
    dest.client = destinationPortId.clientId();
    dest.port = destinationPortId.portId();

    snd_seq_port_subscribe_set_sender(subs, &sender);
    snd_seq_port_subscribe_set_dest(subs, &dest);

    return snd_seq_subscribe_port(handle, subs);
}

int unSubscribePort(snd_seq_t *handle, const MidiClientPortId &senderPortId, const MidiClientPortId &destinationPortId)
{
    snd_seq_addr_t sender, dest;
    snd_seq_port_subscribe_t* subs;
    snd_seq_port_subscribe_alloca(&subs);

    sender.client = senderPortId.clientId();
    sender.port = senderPortId.portId();
    dest.client = destinationPortId.clientId();
    dest.port = destinationPortId.portId();

    snd_seq_port_subscribe_set_sender(subs, &sender);
    snd_seq_port_subscribe_set_dest(subs, &dest);

    return snd_seq_unsubscribe_port(handle, subs);
}
