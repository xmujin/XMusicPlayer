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
    img = new QLabel(coverContainer);
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
    _isHovered = true;
    updateStyle();
    QWidget::enterEvent(event);

}

void MusicListItem::leaveEvent(QEvent *event)
{
    _isHovered = false;
    updateStyle();
    QWidget::leaveEvent(event);

}

void MusicListItem::updateStyle()
{
    // 基础背景颜色（默认）
    QString bgColor = "#f5f5f5";
    QString textColor = "#333";
    bool showBtn = false;
    // 当悬停 播放 选择 时更改背景颜色
    if(_isHovered || _isPlaying || _isSelected)
    {
        bgColor = "#edeeef";
    }

    if(_isPlaying && !_isSelected)
    {
        bgColor = "#f5f5f5";
    }

    if(_isPlaying && _isHovered)
    {
        bgColor = "#edeeef";
    }



    // 如果正在播放，文字高亮为红色，并显示按钮
    if (_isPlaying || _isCurrent) {
        textColor = "#fc3d4a";
        showBtn = true;
    }
    else if (_isHovered) // 如果没有播放但悬停中，显示按钮
    {
        showBtn = true;
    }
    else if (_isSelected) // 如果被选中但没播放，按钮可以隐藏（根据你的需求决定）
    {
        showBtn = false;
    }


    // 应用最终样式
    coverContainer->setStyleSheet(QString("background: %1; border-radius: 0px").arg(bgColor));
    musicNameLabel->setStyleSheet(QString("font-size: 16px; color: %1; padding: 0px 20px 0px; border-radius: 0px; background: %2;")
                                      .arg(textColor, bgColor));

    showBtn ? playBtn->show() : playBtn->hide();
}

void MusicListItem::mousePressEvent(QMouseEvent *event)
{
    _isSelected = true;
    emit itemSelected(this); // 通知外部当前项被选中
    QWidget::mousePressEvent(event);
    updateStyle();
}

bool MusicListItem::isCurrent() const
{
    return _isCurrent;
}

void MusicListItem::setIsCurrent(bool newIsCurrent)
{
    _isCurrent = newIsCurrent;
    updateStyle();
}

bool MusicListItem::isHovered() const
{
    return _isHovered;
}

void MusicListItem::setIsHovered(bool newIsHovered)
{
    _isHovered = newIsHovered;
}

bool MusicListItem::isSelected() const
{
    return _isSelected;
}

void MusicListItem::setIsSelected(bool newIsSelected)
{
    _isSelected = newIsSelected;
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
    _isSelected = true;
    _isCurrent = true;
    playBtn->setIcon(QIcon(":/Icon/pause.png"));
    playBtn->changeIconColor(QColor("#fff"));

    updateStyle();
}

void MusicListItem::pauseMusic()
{
    _isPlaying = false;
    playBtn->setIcon(QIcon(":/Icon/play.png"));
    playBtn->changeIconColor(QColor("#fff"));
    updateStyle();
}

void MusicListItem::stopMusic()
{

    _isPlaying = false;
    _isSelected = false;
    _isCurrent = false;
    playBtn->setIcon(QIcon(":/Icon/play.png"));
    playBtn->changeIconColor(QColor("#fff"));


    updateStyle();
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

    updateStyle();

}

void MusicListItem::setCoverImage(const QPixmap &pix)
{
    img->setPixmap(pix.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

QString MusicListItem::getMusicPath()
{
    return musicPath;
}
