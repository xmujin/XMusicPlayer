#include "musicplayer.h"

MusicPlayer::MusicPlayer(QObject *parent)
    : QObject{parent}
{
    m_player = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_player->setAudioOutput(m_audioOutput);
    setVolume(0.2);
}


MusicPlayer* MusicPlayer::_musicPlayer= nullptr;

MusicPlayer* MusicPlayer::getInstance()
{
    if(_musicPlayer == nullptr)
    {
        _musicPlayer = new MusicPlayer;
    }

    return _musicPlayer;
}

void MusicPlayer::setSource(const QString &musicPath)
{
    m_player->setSource(QUrl::fromLocalFile(musicPath));
}

void MusicPlayer::play()
{

    m_player->play();
}

void MusicPlayer::pause()
{
    m_player->pause();
}

void MusicPlayer::stop()
{
    m_player->stop();
}

void MusicPlayer::setVolume(float volume)
{
    this->volume = volume;
    m_audioOutput->setVolume(volume);
}

QMediaPlayer::PlaybackState MusicPlayer::getPlayState()
{
    return m_player->playbackState();
}

float MusicPlayer::getVolume() const
{
    return volume;
}
