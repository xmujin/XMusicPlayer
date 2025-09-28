#include "musiclistitem.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QDebug>
#include <QFileInfo>

MusicListItem::MusicListItem(QWidget *parent)
    : QWidget{parent}
{


}

MusicListItem::MusicListItem(const QString &musicPath, const QPixmap &musicImg, QWidget *parent) : QWidget{parent}, musicPath{musicPath}
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);  // 控件之间的间距

    // 封面容器（用于悬停按钮）
    coverContainer = new QWidget(this);
    coverContainer->setFixedSize(80, 80);
    coverContainer->setStyleSheet("background: #f5f5f5; border-radius: 0px;");


    // 创建封面图片
    QLabel *img = new QLabel(coverContainer);
    img->setFixedSize(80, 80);
    img->setPixmap(musicImg.scaled(QSize(60, 60), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    img->setAlignment(Qt::AlignCenter);
    img->setStyleSheet("background: transparent;");

    playBtn = new MyPushButton(coverContainer);
    playBtn->setHover(false);

    playBtn->setFixedSize(80, 80);
    playBtn->setIcon(QIcon(":/Icon/play.png"));
    playBtn->setIconSize(QSize(50,50));
    playBtn->changeIconColor("#fff");
    playBtn->setStyleSheet("background: transparent; border: none;");
    playBtn->setCursor(Qt::PointingHandCursor);
    playBtn->hide();


    connect(playBtn, &QPushButton::clicked, this, [this]() {
        emit playClicked(this);
    });

    QFileInfo fileInfo(musicPath);
    musicNameLabel = new QLabel(fileInfo.baseName(), this);
    musicNameLabel->setStyleSheet("font-size: 16px; color: #333; padding: 0px 20px 0px; border-radius: 0px;background: #f5f5f5;");

    layout->addWidget(coverContainer);
    layout->addWidget(musicNameLabel);

    setPlaying(false);

}

void MusicListItem::enterEvent(QEnterEvent *event)
{
    if(!_isPlaying)
    {
        playBtn->show();
        coverContainer->setStyleSheet("background: #edeeef; border-radius: 0px");
        musicNameLabel->setStyleSheet("font-size: 16px; color: #333; padding: 0px 20px 0px; border-radius: 0px;background: #edeeef;");

    }

}

void MusicListItem::leaveEvent(QEvent *event)
{

    if(!_isPlaying)
    {
        playBtn->hide();
        coverContainer->setStyleSheet("background: #f5f5f5; border-radius: 0px");
        musicNameLabel->setStyleSheet("font-size: 16px; color: #333; padding: 0px 20px 0px; border-radius: 0px;background: #f5f5f5;");

    }

}

void MusicListItem::setRow(int newRow)
{
    row = newRow;
}

int MusicListItem::getRow() const
{
    return row;
}

void MusicListItem::playMusic()
{
    _isPlaying = true;
    playBtn->show();
    playBtn->setIcon(QIcon(":/Icon/pause.png"));
    playBtn->changeIconColor(QColor("#fff"));


    coverContainer->setStyleSheet("background: #edeeef; border-radius: 0px");
    musicNameLabel->setStyleSheet("font-size: 16px; color: #333; padding: 0px 20px 0px; border-radius: 0px;background: #edeeef;");

}

void MusicListItem::pauseMusic()
{
    _isPlaying = false;
}

void MusicListItem::stopMusic()
{
    _isPlaying = false;
    playBtn->hide();

    coverContainer->setStyleSheet("background: #f5f5f5; border-radius: 0px");
    musicNameLabel->setStyleSheet("font-size: 16px; color: #333; padding: 0px 20px 0px; border-radius: 0px;background: #f5f5f5;");
}

void MusicListItem::showTips()
{
    playBtn->show();
    coverContainer->setStyleSheet("background: #edeeef; border-radius: 0px");
    musicNameLabel->setStyleSheet("font-size: 16px; color: #333; padding: 0px 20px 0px; border-radius: 0px;background: #edeeef;");

}

bool MusicListItem::isPlaying()
{
    return _isPlaying;
}

void MusicListItem::setPlaying(bool state)
{
    _isPlaying = state;

    if(_isPlaying)
    {
        playBtn->setIcon(QIcon(":/Icon/pause.png"));
        playBtn->changeIconColor(QColor("#fff"));
    }
    else
    {
        playBtn->setIcon(QIcon(":/Icon/play.png"));
        playBtn->changeIconColor(QColor("#fff"));
    }
}

QString MusicListItem::getMusicPath()
{
    return musicPath;
}
