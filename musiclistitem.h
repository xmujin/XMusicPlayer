#ifndef MUSICLISTITEM_H
#define MUSICLISTITEM_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "mypushbutton.h"


class MusicListItem : public QWidget
{
    Q_OBJECT
public:
    explicit MusicListItem(QWidget *parent = nullptr);
    explicit MusicListItem(const QString& musicPath, const QPixmap& musicImg, QWidget *parent = nullptr);
    void playMusic();
    void pauseMusic();
    void stopMusic();

    void showTips();

    bool isPlaying();
    void setPlaying(bool state);

    QString getMusicPath();


    int getRow() const;

    void setRow(int newRow);

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:


private:
    QWidget* coverContainer; // 封面容器  包含图片和按钮
    QLabel *musicNameLabel;
    MyPushButton *playBtn;

    QString musicPath;

    bool _isPlaying = false;

    int row;

signals:
    void playClicked(MusicListItem* self);

};

#endif // MUSICLISTITEM_H
