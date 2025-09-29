#include "musiclist.h"
#include <QDebug>

MusicList::MusicList(QWidget *parent) : QListWidget{parent}
{

}

void MusicList::mousePressEvent(QMouseEvent *event)
{
    QListWidgetItem *item = itemAt(event->pos());
    if (item) {
        emit itemClicked(item);
    } else {
        qDebug() << "我触发了";
        QListWidget::mousePressEvent(event);

    }
}
