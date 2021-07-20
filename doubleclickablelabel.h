#ifndef DOUBLECLICKABLELABEL_H
#define DOUBLECLICKABLELABEL_H

#include <QLabel>

class DoubleClickableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit DoubleClickableLabel(const QString &text, QWidget *parent = nullptr) : QLabel(text, parent) {}
    explicit DoubleClickableLabel(QWidget *parent = nullptr) : QLabel(parent) {}

signals:
    void doubleClicked();

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;


};

#endif // DOUBLECLICKABLELABEL_H
