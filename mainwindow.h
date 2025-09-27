#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mypushbutton.h"
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QListWidget>

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

public slots:
    void handlePlaySlot();
    void handleModeSlot();
    void handleNextSlot();
    void handleShowListSlot();
public:
    void InitButton();
    void setButtonIcon(MyPushButton* btn, const QString& iconPath);
private:
    void loadMusicDir(const QString& filePath);
    void startMusic();

    void showAnimation(QWidget* widget);
    void hideAnimation(QWidget* widget);

private:
    MyPushButton* listBtn;
    MyPushButton* modeBtn;
    MyPushButton* nextBtn;
    MyPushButton* playBtn;
    MyPushButton* prevBtn;

    QListWidget* musicList;

    QMediaPlayer* m_player;
    QAudioOutput*  m_audioOutput;
    PlayMode m_mode;
    QString m_musicDir;

    QWidget* playlistContainer;

    bool m_isListShow;


};
#endif // MAINWINDOW_H
