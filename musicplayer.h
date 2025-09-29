#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H
#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>


/**
 * @brief 音乐播放器类，单例模式实现
 */
class MusicPlayer : public QObject
{
    Q_OBJECT
protected:
    explicit MusicPlayer(QObject *parent = nullptr);

public:
    // 禁止拷贝构造和赋值构造
    MusicPlayer(const MusicPlayer& other) = delete;
    MusicPlayer& operator=(const MusicPlayer& other) = delete;

    static MusicPlayer *getInstance();


    void setSource(const QString& musicPath);
    void play();
    void pause();
    void stop();

    void setVolume(float volume);

    QMediaPlayer::PlaybackState getPlayState();


    float getVolume() const;

protected:
    static MusicPlayer* _musicPlayer;

private:
    QMediaPlayer* m_player;
    QAudioOutput* m_audioOutput;

    float volume;

signals:
};

#endif // MUSICPLAYER_H
