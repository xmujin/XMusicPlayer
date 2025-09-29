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

    bool isPlaying();
    void setPlaying(bool state);

    void setCoverImage(const QPixmap &pix);

    QString getMusicPath();


    int getRow() const;

    void setRow(int newRow);

    bool isSelected() const;
    void setIsSelected(bool newIsSelected);

    void updateStyle();

    bool isHovered() const;
    void setIsHovered(bool newIsHovered);

    bool isCurrent() const;
    void setIsCurrent(bool newIsCurrent);

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:


private:
    QWidget* coverContainer; // 封面容器  包含图片和按钮
    QLabel *musicNameLabel;
    MyPushButton *playBtn;
    QLabel *img;

    QString musicPath;

    bool _isPlaying = false;
    bool _isHovered = false;
    bool _isSelected = false;
    bool _isCurrent = false;

    int row;

signals:
    void playClicked(MusicListItem* self);
    void itemSelected(MusicListItem* self);

};

#endif // MUSICLISTITEM_H
