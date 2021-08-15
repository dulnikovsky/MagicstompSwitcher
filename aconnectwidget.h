#ifndef ACONNECTWIDGET_H
#define ACONNECTWIDGET_H

#include <QWidget>
class QPlainTextEdit;

class AConnectWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AConnectWidget(QWidget *parent = nullptr);

protected:
    void showEvent(QShowEvent *event) override;

private:
    QPlainTextEdit *aconnectDisplay;

    void runAconnect();

};

#endif // ACONNECTWIDGET_H
