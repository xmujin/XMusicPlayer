#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H
#include <QPushButton>
#include <QEvent>
#include <QColor>
class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyPushButton(QWidget *parent = nullptr);

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
public:
    void changeIconColor(QColor color);
    QColor getNormalColor() { return normal; }
    QColor getHoverColor() { return hover; }

signals:

private:
    QColor normal {"#525a6c"};
    QColor hover {"#242d41"};

};

#endif // MYPUSHBUTTON_H
