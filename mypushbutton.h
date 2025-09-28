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
    void setHover(bool v);


signals:

private:
    QColor normal {"#525a6c"};
    QColor hover {"#242d41"};
    bool enableHover = true;

};

#endif // MYPUSHBUTTON_H
