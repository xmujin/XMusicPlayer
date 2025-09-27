#include "mypushbutton.h"
#include <QPainter>
#include <QDebug>
MyPushButton::MyPushButton(QWidget *parent)
    : QPushButton{parent}
{}

void MyPushButton::enterEvent(QEnterEvent *event)
{
    QPushButton::enterEvent(event);
    changeIconColor(hover);
}

void MyPushButton::leaveEvent(QEvent *event)
{
    QPushButton::leaveEvent(event);
    changeIconColor(normal);
}

void MyPushButton::changeIconColor(QColor color)
{
    // 获取按钮的当前图标（QIcon）
    QIcon buttonIcon = this->icon();
    QPixmap originalPixmap = buttonIcon.pixmap(buttonIcon.actualSize(QSize(1000, 1000)));
    // 创建透明背景的 QPixmap
    QPixmap coloredPixmap(originalPixmap.size());
    coloredPixmap.fill(Qt::transparent);

    // 使用 QPainter 绘制并染色
    QPainter painter(&coloredPixmap);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.drawPixmap(0, 0, originalPixmap);  // 绘制原始图标
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(coloredPixmap.rect(), color);
    painter.end();

    // 将染色后的 QPixmap 设置回按钮
    this->setIcon(QIcon(coloredPixmap));
}
