#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

class QPlainTextEdit;
class QSpinBox;

class PreferencesDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PreferencesDialog(QWidget *parent = nullptr);

    void setMidiChannel(unsigned int channel);
    unsigned int MidiChannel() const;
signals:
    void midiChannelChanged(unsigned int channel);
private:
    QSpinBox *channelSpinBox;

    void runAconnect();
    QPlainTextEdit *aconnectDisplay;

private slots:
    void channelSpinBoxChanged(int val);
};

#endif // PREFERENCESDIALOG_H
