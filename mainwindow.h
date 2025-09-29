#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mypushbutton.h"
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QListWidget>
#include "musiclistitem.h"
#include "musicplayer.h"
#include "musiclist.h"
#include <QSlider>


enum PlayMode
{
    ORDER_MODE,
    RANDOM_MODE
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void showEvent(QShowEvent *event) override;

public slots:
    void handlePlaySlot();
    void handleModeSlot();
    void handleNextSlot();
    void handleShowListSlot();
    void handleCurrentRowChanged(int row);
    void onVolumnBtnClicked();

public:
    void InitButton();
    void setButtonIcon(MyPushButton* btn, const QString& iconPath);
private:
    void loadMusicDir(const QString& filePath);
    void startMusic();

    void showAnimation(QWidget* widget);
    void hideAnimation(QWidget* widget);

private:

    MusicPlayer* musicPlayer;
    MyPushButton* listBtn;
    MyPushButton* modeBtn;
    MyPushButton* nextBtn;
    MyPushButton* playBtn;
    MyPushButton* prevBtn;
    MyPushButton* volumnBtn;

    MusicList* musicList;

    QSlider *volumeSlider;


    PlayMode m_mode;
    QString m_musicDir;

    QWidget* playlistContainer;

    bool m_isListShow;

    // 保存当前播放的项
    MusicListItem *currentPlayingItem = nullptr;
    // 保存当前播放的项
    MusicListItem *currentSelectedItem = nullptr;


};
#endif // MAINWINDOW_H
