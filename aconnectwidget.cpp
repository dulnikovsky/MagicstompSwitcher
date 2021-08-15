#include "aconnectwidget.h"

#include <QProcess>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPlainTextEdit>

AConnectWidget::AConnectWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainLyt = new QVBoxLayout();

    aconnectDisplay = new QPlainTextEdit();
    aconnectDisplay->setReadOnly(true);
    QFont aconnectDisplayFont = aconnectDisplay->font();
    aconnectDisplayFont.setPointSize(7);
    aconnectDisplay->setFont(aconnectDisplayFont);

    mainLyt->addWidget(aconnectDisplay);

    QPushButton *closeButton = new QPushButton(tr("Close"));
    connect(closeButton, SIGNAL(pressed()), this, SLOT(close()));

    QHBoxLayout *buttonLyt = new QHBoxLayout();
    buttonLyt->addWidget(closeButton);
    buttonLyt->addStretch(4);

    mainLyt->addLayout(buttonLyt);

    setLayout(mainLyt);
}

void AConnectWidget::showEvent(QShowEvent *)
{
    runAconnect();
}

void AConnectWidget::runAconnect()
{
    QString program = "aconnect";
    QStringList arguments;
    arguments << "-l";

    QProcess *myProcess = new QProcess(this);
    QObject::connect(
        myProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        [myProcess, this](int ) {
            QByteArray output = myProcess->readAllStandardOutput();
            this->aconnectDisplay->setPlainText(QString(output));
            myProcess->deleteLater();
        }
    );
    myProcess->start(program, arguments);
}
