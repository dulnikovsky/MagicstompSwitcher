#include "preferencesdialog.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QProcess>
#include <QPlainTextEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QCheckBox>

#include "msswitcherthread.h"

extern const MSSwitcherThread *switcherThread;

PreferencesDialog::PreferencesDialog(QWidget *parent) : QWidget(parent)
{
    channelSpinBox = new QSpinBox();
    channelSpinBox->setMaximum(16);
    channelSpinBox->setMinimum(0);
    channelSpinBox->setSpecialValueText(QStringLiteral("OMNI"));
    channelSpinBox->setValue(switcherThread->MidiChannel());
    connect(channelSpinBox, SIGNAL(valueChanged(int)),this, SLOT(channelSpinBoxChanged(int)));

    gainCCNUmberSpinBox = new QSpinBox();
    gainCCNUmberSpinBox->setMaximum(127);
    gainCCNUmberSpinBox->setMinimum(-1);
    gainCCNUmberSpinBox->setSpecialValueText(QStringLiteral("OFF"));
    gainCCNUmberSpinBox->setValue(switcherThread->GainCCNumber());
    connect(gainCCNUmberSpinBox, SIGNAL(valueChanged(int)),this, SIGNAL(gainCCNumberChanged(int)));

    masterCCNUmberSpinBox = new QSpinBox();
    masterCCNUmberSpinBox->setMaximum(127);
    masterCCNUmberSpinBox->setMinimum(-1);
    masterCCNUmberSpinBox->setSpecialValueText(QStringLiteral("OFF"));
    masterCCNUmberSpinBox->setValue(switcherThread->MasterCCNumber());
    connect(masterCCNUmberSpinBox, SIGNAL(valueChanged(int)),this, SIGNAL(masterCCNumberChanged(int)));

    effectLevelCCNUmberSpinBox = new QSpinBox();
    effectLevelCCNUmberSpinBox->setMaximum(127);
    effectLevelCCNUmberSpinBox->setMinimum(-1);
    effectLevelCCNUmberSpinBox->setSpecialValueText(QStringLiteral("OFF"));
    effectLevelCCNUmberSpinBox->setValue(switcherThread->EffectCCNumber());
    connect(effectLevelCCNUmberSpinBox, SIGNAL(valueChanged(int)),this, SIGNAL(effectLevelCCNumberChanged(int)));

    QCheckBox *midiThroughCheckBox = new QCheckBox();
    midiThroughCheckBox->setCheckable(true);
    midiThroughCheckBox->setChecked(switcherThread->MidiThrough());
    connect(midiThroughCheckBox, SIGNAL(toggled(bool)),this, SIGNAL(midiThroughChanged(bool)));

    QFormLayout *formLyt = new QFormLayout();
    formLyt->addRow(new QLabel(QStringLiteral("MIDI Channel :")), channelSpinBox);
    formLyt->addRow(new QLabel(QStringLiteral("Gain CC Number :")), gainCCNUmberSpinBox);
    formLyt->addRow(new QLabel(QStringLiteral("Master CC Number :")), masterCCNUmberSpinBox);
    formLyt->addRow(new QLabel(QStringLiteral("8 Band Delay Effect Level CC Number :")), effectLevelCCNUmberSpinBox);
    formLyt->addRow(new QLabel(QStringLiteral("MIDI Through :")), midiThroughCheckBox);

    /*aconnectDisplay = new QPlainTextEdit();
    aconnectDisplay->setReadOnly(true);
    QFont aconnectDisplayFont = aconnectDisplay->font();
    aconnectDisplayFont.setPointSize(5);
    aconnectDisplay->setFont(aconnectDisplayFont);*/

    QPushButton *closeButton = new QPushButton(tr("Close"));
    connect(closeButton, SIGNAL(pressed()), this, SLOT(close()));

    QHBoxLayout *mainLyt = new QHBoxLayout();
    mainLyt->addLayout(formLyt);
    //mainLyt->addWidget(aconnectDisplay);
    mainLyt->addWidget(closeButton);

    setLayout(mainLyt);

    //runAconnect();

}

/*void PreferencesDialog::runAconnect()
{
    QString program = "aconnect";
    QStringList arguments;
    arguments << "-l";

    QProcess *myProcess = new QProcess(this);
    QObject::connect(
        myProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        [myProcess, this](int exitCode) {
            QByteArray output = myProcess->readAllStandardOutput();
            this->aconnectDisplay->setPlainText(QString(output));
            myProcess->deleteLater();
        }
    );
    myProcess->start(program, arguments);
}*/

void PreferencesDialog::setMidiChannel(unsigned int channel)
{
    channelSpinBox->setValue(channel);
}

unsigned int PreferencesDialog::MidiChannel() const
{
    return channelSpinBox->value();
}

void PreferencesDialog::channelSpinBoxChanged(int val)
{
    emit midiChannelChanged(val);
}
