#include "musiclistitem.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>


MusicListItem::MusicListItem(QWidget *parent)
    : QWidget{parent}
{


}

MusicListItem::MusicListItem(const QString &musicName, const QPixmap &musicImg, QWidget *parent) : QWidget{parent}
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

    playBtn = new QPushButton(coverContainer);
    playBtn->setFixedSize(80, 80);
    playBtn->setIcon(QIcon(":/Icon/play.png"));
    playBtn->setIconSize(QSize(50,50));
    playBtn->setStyleSheet("background: transparent; border: none;");
    playBtn->setCursor(Qt::PointingHandCursor);
    playBtn->hide();



    musicNameLabel = new QLabel(musicName, this);
    musicNameLabel->setStyleSheet("font-size: 16px; color: #333; padding: 0px 20px 0px; border-radius: 0px;background: #f5f5f5;");

    layout->addWidget(coverContainer);
    layout->addWidget(musicNameLabel);



}

void MusicListItem::enterEvent(QEnterEvent *event)
{
    playBtn->show();
    coverContainer->setStyleSheet("background: #edeeef; border-radius: 0px");
    musicNameLabel->setStyleSheet("font-size: 16px; color: #333; padding: 0px 20px 0px; border-radius: 0px;background: #edeeef;");
}

void MusicListItem::leaveEvent(QEvent *event)
{
    playBtn->hide();
    coverContainer->setStyleSheet("background: #f5f5f5; border-radius: 0px");
    musicNameLabel->setStyleSheet("font-size: 16px; color: #333; padding: 0px 20px 0px; border-radius: 0px;background: #f5f5f5;");
}
