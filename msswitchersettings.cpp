#include "msswitchersettings.h"

MSSwitcherSettings::MSSwitcherSettings(QObject *parent) : QSettings(parent)
{
}

void MSSwitcherSettings::setMidiChannel(unsigned int val)
{
    setValue(QStringLiteral("MIDIChannel"), val);
    sync();
}
void MSSwitcherSettings::setGainCCNumber(int val)
{
    setValue(QStringLiteral("GainCCNumber"), val);
    sync();
}
void MSSwitcherSettings::setMasterCCNumber(int val)
{
    setValue(QStringLiteral("MasterCCNumber"), val);
    sync();
}
void MSSwitcherSettings::setEffectCCNumber(int val)
{
    setValue(QStringLiteral("EffectCCNUmber"), val);
    sync();
}
void MSSwitcherSettings::setMidiThrough(bool val)
{
    setValue(QStringLiteral("MidiThrough"), val);
    sync();
}

unsigned int MSSwitcherSettings::MidiChannel() const
{
    return value(QStringLiteral("MIDIChannel"),0).toUInt();
}
int MSSwitcherSettings::GainCCNumber() const
{
    return value(QStringLiteral("GainCCNumber"),-1).toInt();
}
int MSSwitcherSettings::MasterCCNumber() const
{
    return value(QStringLiteral("MasterCCNumber"),-1).toInt();
}
int MSSwitcherSettings::EffectCCNumber() const
{
    return value(QStringLiteral("EffectCCNUmber"),-1).toInt();
}
bool MSSwitcherSettings::MidiThrough() const
{
    return value(QStringLiteral("MidiThrough"),false).toBool();
}
