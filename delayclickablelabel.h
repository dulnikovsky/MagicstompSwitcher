#ifndef DELAYCLICKABLELABEL_H
#define DELAYCLICKABLELABEL_H

#include <QLabel>
class QTimer;

class DelayClickableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit DelayClickableLabel(const QString &text, QWidget *parent = nullptr) : QLabel(text, parent) {}
    explicit DelayClickableLabel(QWidget *parent = nullptr) : QLabel(parent) {}

    void setDelayTime( int ms) { delayTime = ms;}
    int DelayTime() const { return delayTime; }

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QTimer *timer{nullptr};
    int delayTime{1000};
};

#endif // DELAYCLICKABLELABEL_H
