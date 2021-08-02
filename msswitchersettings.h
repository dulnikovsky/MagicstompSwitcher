#ifndef MSSWITCHERSETTINGS_H
#define MSSWITCHERSETTINGS_H

#include <QSettings>

class MSSwitcherSettings : public QSettings
{
    Q_OBJECT
public:
    explicit MSSwitcherSettings(QObject *parent = nullptr);

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
};

#endif // MSSWITCHERSETTINGS_H
