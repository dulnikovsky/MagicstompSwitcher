#include "delayclickablelabel.h"

#include <QTimer>

void DelayClickableLabel::mousePressEvent(QMouseEvent *)
{
    if(timer != nullptr)
        return;

    timer = new QTimer(this);
    timer->setSingleShot(true);

    connect(timer, &QTimer::timeout, [this](){

        emit clicked();
        timer->deleteLater();
        timer = nullptr;

    });

    timer->start(delayTime);
}

void DelayClickableLabel::mouseReleaseEvent(QMouseEvent *)
{
    if(timer == nullptr)
        return;

    timer->stop();
    timer->deleteLater();
    timer = nullptr;
}
