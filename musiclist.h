#ifndef MUSICLIST_H
#define MUSICLIST_H

#include <QListWidget>
#include <QWidget>
#include <QMouseEvent>
#include <QListWidgetItem>
#include <QObject>

class MusicList : public QListWidget
{
    Q_OBJECT
public:
    explicit MusicList(QWidget *parent = nullptr);

protected:

    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *e) override {

    }

};

#endif // MUSICLIST_H
