#include "doubleclickablelabel.h"

void DoubleClickableLabel::mouseDoubleClickEvent(QMouseEvent *)
{
    emit doubleClicked();
}

void DoubleClickableLabel::mousePressEvent(QMouseEvent *)
{
    emit doubleClicked();
}
