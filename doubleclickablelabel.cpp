#include "doubleclickablelabel.h"

void DoubleClickableLabel::mouseDoubleClickEvent(QMouseEvent *)
{
    emit doubleClicked();
}
