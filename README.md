# MagicstompSwitcher for Linux

## Introduction ( Work in Progress !!!)

MagicstompSwitcher makes it possible to switch patches of Yamaha's Magicstomp using MIDI Program Change Messages.

## How it works

At startup this programm scans for all connected Magicstomps and copies all user patches of each of them into memory. It also scans for other hardware MIDI devices ( to which a MIDI Foot Controller is connected ). Incoming Program Change messages will trigger sending according patch to each Magictomp using SysEx messages.
