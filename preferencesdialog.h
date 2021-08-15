#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QWidget>

class QPlainTextEdit;
class QSpinBox;

class PreferencesDialog : public QWidget
{
    Q_OBJECT
public:
    explicit PreferencesDialog(QWidget *parent = nullptr);

    void setMidiChannel(unsigned int channel);
    unsigned int MidiChannel() const;
signals:
    void midiChannelChanged(unsigned int channel);
    void gainCCNumberChanged(int num);
    void masterCCNumberChanged(int num);
    void effectLevelCCNumberChanged(int num);
    void midiThroughChanged(bool);
private:
    QSpinBox *channelSpinBox;
    QSpinBox *gainCCNUmberSpinBox;
    QSpinBox *masterCCNUmberSpinBox;
    QSpinBox *effectLevelCCNUmberSpinBox;

    QWidget *aconnectWidget{nullptr};

private slots:
    void channelSpinBoxChanged(int val);
    void showAConnectWidget();
};

#endif // PREFERENCESDIALOG_H
