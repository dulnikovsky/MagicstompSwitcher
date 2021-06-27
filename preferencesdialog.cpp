#include "preferencesdialog.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QProcess>
#include <QPlainTextEdit>
#include <QSpinBox>

PreferencesDialog::PreferencesDialog(QWidget *parent) : QDialog(parent)
{
    channelSpinBox = new QSpinBox();
    channelSpinBox->setMaximum(16);
    channelSpinBox->setMinimum(0);
    channelSpinBox->setSpecialValueText("OMNI");
    connect(channelSpinBox, SIGNAL(valueChanged(int)),this, SLOT(channelSpinBoxChanged(int)));

    QFormLayout *formLyt = new QFormLayout();
    formLyt->addRow(new QLabel("MIDI Channel :"), channelSpinBox);


    aconnectDisplay = new QPlainTextEdit();
    aconnectDisplay->setReadOnly(true);
    QFont aconnectDisplayFont = aconnectDisplay->font();
    aconnectDisplayFont.setPointSize(5);
    aconnectDisplay->setFont(aconnectDisplayFont);

    QHBoxLayout *mainLyt = new QHBoxLayout();
    mainLyt->addLayout(formLyt);
    mainLyt->addWidget(aconnectDisplay);

    setLayout(mainLyt);

    runAconnect();

}

void PreferencesDialog::runAconnect()
{
    QString program = "aconnect";
    QStringList arguments;
    arguments << "-l";

    QProcess *myProcess = new QProcess(this);
    QObject::connect(
        myProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        [myProcess, this](int /*exitCode*/) {
            QByteArray output = myProcess->readAllStandardOutput();
            this->aconnectDisplay->setPlainText(QString(output));
            myProcess->deleteLater();
        }
    );
    myProcess->start(program, arguments);
}

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
