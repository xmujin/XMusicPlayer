#include "mainwindow.h"
#include <QHBoxLayout>
#include <QListWidget>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QPropertyAnimation>
#include <QLabel>
#include <QApplication>
#include <QMediaMetaData>
#include "musiclistitem.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_mode(ORDER_MODE), m_isListShow(false)
{
    setWindowIcon(QIcon(":/Icon/音乐.png"));
    setWindowTitle("音乐播放器");
    setFixedSize(800, 600);

    listBtn = new MyPushButton(this);
    modeBtn = new MyPushButton(this);
    nextBtn = new MyPushButton(this);
    playBtn = new MyPushButton(this);
    prevBtn = new MyPushButton(this);
    volumnBtn = new MyPushButton(this);

    volumnBtn->setVisible(true);

    volumeSlider = new QSlider(Qt::Vertical, this);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(20);
    volumeSlider->setFixedSize(18, 84);



    QWidget *centralWidget = new QWidget(this);
    volumeSlider->raise();

    // 设置主垂直布局
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addStretch();



    // 水平布局
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(prevBtn);
    buttonLayout->addWidget(playBtn);
    buttonLayout->addWidget(nextBtn);
    buttonLayout->addWidget(modeBtn);
    buttonLayout->addWidget(listBtn);
    buttonLayout->addWidget(volumnBtn);

    // 在垂直布局中添加水平布局
    mainLayout->addLayout(buttonLayout);
    mainLayout->setContentsMargins(0, 0, 0, 30);
    setCentralWidget(centralWidget);


    // 创建侧边播放列表
    playlistContainer = new QWidget(this);
    playlistContainer->setVisible(false);
    playlistContainer->setFixedWidth(300);
    playlistContainer->setFixedHeight(400);
    playlistContainer->setStyleSheet(R"(
        background: #f5f5f5;
        border-radius: 20px;
    )");



    QVBoxLayout *playlistLayout = new QVBoxLayout(playlistContainer);
    playlistLayout->setContentsMargins(0, 0, 0, 0);  // 关键：移除边距
    playlistLayout->setSpacing(0);                   // 关键：移除间距




    musicList = new MusicList(this);
    musicList->setStyleSheet(R"(
        QListWidget {
                border: none;
                background-color: #f5f5f5;
                padding: 0px;
                border-radius: 0px;
                outline: 0;
            }


        QListWidget::item:selected {
            background: transparent;
            color: black;

        }

        QListWidget::item:hover {
            background: transparent;
            color: black;
        }


    )");

    musicList->setSelectionMode(QAbstractItemView::NoSelection); // 禁止选中项


    connect(musicList, &MusicList::itemSelectionChanged, this, [=](){
        qDebug() << "选中项发生了改变";
    });
    // 1. 添加标题（QLabel）
    QLabel *titleLabel = new QLabel("播放列表", this);
    titleLabel->setFixedHeight(50);
    titleLabel->setAlignment(Qt::AlignLeft);
    titleLabel->setStyleSheet(R"(
        QLabel {
            font-size: 18px;
            font-weight: bold;
            color: #333;
            padding: 10px;
        }
    )");

    playlistLayout->addWidget(titleLabel);
    playlistLayout->addWidget(musicList);

    playlistContainer->show();
    playlistContainer->hide();


    InitButton();

    musicPlayer = MusicPlayer::getInstance();


    m_musicDir = "D:\\xiangxun\\Music\\";
    loadMusicDir(m_musicDir);
    connect(musicList, &MusicList::currentRowChanged, this, &MainWindow::handleCurrentRowChanged);
    musicList->setCurrentRow(0);

    QPoint inWindow = volumnBtn->mapTo(this, QPoint(0, 0));
    qDebug() << "窗口内坐标:" << inWindow;


    connect(volumeSlider, &QSlider::valueChanged, this, [this](int value){
        musicPlayer->setVolume(value / 100.0);
    } );





}

MainWindow::~MainWindow() {}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);

    volumeSlider->move(volumnBtn->x(), volumnBtn->y() - 100);

}




void MainWindow::InitButton()
{
    // 上一首
    setButtonIcon(prevBtn, ":/Icon/prev.png");
    // 播放
    setButtonIcon(playBtn, ":/Icon/play.png");
    // 下一首
    setButtonIcon(nextBtn, ":/Icon/next.png");
    // 播放模式
    setButtonIcon(modeBtn, ":/Icon/order.png");
    // 播放列表
    setButtonIcon(listBtn, ":/Icon/list.png");
    // 音量
    setButtonIcon(volumnBtn, ":/Icon/volumeLow.png");



    connect(playBtn, &QPushButton::clicked, this, &MainWindow::handlePlaySlot);
    connect(modeBtn, &QPushButton::clicked, this, &MainWindow::handleModeSlot);
    connect(nextBtn, &QPushButton::clicked, this, &MainWindow::handleNextSlot);
    connect(listBtn, &QPushButton::clicked, this, &MainWindow::handleShowListSlot);
    connect(volumnBtn, &QPushButton::clicked, this, &MainWindow::onVolumnBtnClicked);
}



void MainWindow::setButtonIcon(MyPushButton *btn, const QString &iconPath)
{

    btn->setFixedSize(50, 50);
    btn->setStyleSheet("background-color: transparent;border: none;");
    btn->setIcon(QIcon(iconPath));
    btn->setIconSize(QSize(btn->width(), btn->height()));
    btn->changeIconColor(btn->getNormalColor());
    btn->setFlat(true);
    btn->setCursor(Qt::PointingHandCursor);
}



void MainWindow::loadMusicDir(const QString &filePath)
{
    QDir dir(filePath);
    if(dir.exists() == false)
    {
        QMessageBox::warning(this, "文件夹", "文件夹打开失败");
        return;
    }


    int i = 0;
    QFileInfoList fileList = dir.entryInfoList(QDir::Files);
    for(auto element : fileList)
    {
        if(element.suffix() == "mp3")
        {
            // 创建ListWidget的item项
            QListWidgetItem *item = new QListWidgetItem(musicList);
            item->setSizeHint(QSize(0, 80));
            QPixmap pixmap(":/Icon/音符.png");



            MusicListItem* musicItem = new MusicListItem(element.absoluteFilePath(), pixmap, this);
            musicItem->setRow(i);


            auto *player = new QMediaPlayer(musicItem); // 让player的生命周期跟item一样
            player->setSource(QUrl::fromLocalFile(element.absoluteFilePath()));

            // 3. 等封面解析出来后，通知item更新图片
            connect(player, &QMediaPlayer::metaDataChanged, musicItem, [musicItem, player]() {
                QVariant coverVariant = player->metaData().value(QMediaMetaData::ThumbnailImage);
                if (coverVariant.canConvert<QImage>()) {
                    QPixmap cover = QPixmap::fromImage(coverVariant.value<QImage>());
                    musicItem->setCoverImage(cover);  // 写一个public方法更新图标
                }
            });


            connect(musicItem, &MusicListItem::itemSelected, this, [this](MusicListItem *clickedItem) {
                // 将之前选中项清空样式
                if(currentSelectedItem && currentSelectedItem != clickedItem)
                {
                    currentSelectedItem->setIsSelected(false);
                    currentSelectedItem->updateStyle();
                }
                // 新的选中项自动设置了选中状态  更新样式即可
                currentSelectedItem = clickedItem;
                currentSelectedItem->updateStyle();
            });
            connect(musicItem, &MusicListItem::playClicked, this, [this](MusicListItem *clickedItem) {
                if(currentPlayingItem && currentPlayingItem != clickedItem)
                {

                    // 停止之前的音乐播放
                    currentPlayingItem->stopMusic();
                    // 将当前音乐设置为源
                    currentPlayingItem = clickedItem;
                    musicPlayer->setSource(currentPlayingItem->getMusicPath());
                }
                handlePlaySlot();

            });
            musicList->setItemWidget(item, musicItem);
        }
        i++;
    }
}

void MainWindow::startMusic()
{


}

/**
 * @brief MainWindow::handlePlaySlot 处理音乐暂停与播放
 */
void MainWindow::handlePlaySlot()
{
    if(musicPlayer->getPlayState() == QMediaPlayer::PlayingState)
    {
        // 暂停音乐
        musicPlayer->pause();
        playBtn->setIcon(QIcon(":/Icon/play.png"));

        // 设置当前播放的音乐暂停
        currentPlayingItem->pauseMusic();
    }
    else
    {
        // 播放音乐
        musicPlayer->play();
        playBtn->setIcon(QIcon(":/Icon/pause.png"));

        // 设置播放项为当前选择项
        currentSelectedItem = currentPlayingItem;
        // 设置播放列表为播放状态
        currentPlayingItem->playMusic();
    }



}


void MainWindow::handleNextSlot()
{
    // 获取当前正在播放的音乐
    int currentRow = currentPlayingItem->getRow();
    int nextRow = 0;
    if(m_mode == ORDER_MODE)
    {
        nextRow = (currentRow + 1) % musicList->count();
    }
    else
    {
        do {
            nextRow = QRandomGenerator::global()->bounded(musicList->count());
        } while (currentRow == nextRow && musicList->count() != 1);
    }

    // 将之前的音乐项取消播放
    currentPlayingItem->stopMusic();
    // 设置新的播放音乐
    musicList->setCurrentRow(nextRow);
    // 同步主页图标
    handlePlaySlot();
}

void MainWindow::handleModeSlot()
{
    if(m_mode == ORDER_MODE)
    {
        m_mode = RANDOM_MODE;
        modeBtn->setIcon(QIcon(":/Icon/random.png"));
        modeBtn->changeIconColor(modeBtn->getHoverColor());
    }
    else
    {
        m_mode = ORDER_MODE;
        modeBtn->setIcon(QIcon(":/Icon/order.png"));
        modeBtn->changeIconColor(modeBtn->getHoverColor());

    }
}


void MainWindow::showAnimation(QWidget *widget)
{
    QPropertyAnimation anim(widget, "pos");
    anim.setDuration(100);
    anim.setStartValue(QPoint(this->width(), 50));
    anim.setEndValue(QPoint(this->width() - widget->width(), 50));
    anim.start();

    QEventLoop loop;
    connect(&anim, &QPropertyAnimation::finished, &loop, &QEventLoop::quit);
    loop.exec();

}



void MainWindow::hideAnimation(QWidget *widget)
{
    QPropertyAnimation anim(widget, "pos");
    anim.setDuration(100);
    anim.setStartValue(QPoint(this->width() - widget->width(), 50));
    anim.setEndValue(QPoint(this->width(), 50));
    anim.start();

    QEventLoop loop;
    connect(&anim, &QPropertyAnimation::finished, &loop, &QEventLoop::quit);
    loop.exec();
}



void MainWindow::handleShowListSlot()
{
    if(m_isListShow == false)
    {
        playlistContainer->show();
        showAnimation(playlistContainer);
        m_isListShow = true;
    }
    else
    {
        hideAnimation(playlistContainer);
        playlistContainer->hide();
        m_isListShow = false;
    }
}

void MainWindow::handleCurrentRowChanged(int row)
{
    QListWidgetItem* item = musicList->currentItem();
    MusicListItem* music = qobject_cast<MusicListItem*>(musicList->itemWidget(item));
    musicPlayer->setSource(music->getMusicPath());
    currentPlayingItem = music;
    qDebug() << "当前项改变";
}

void MainWindow::onVolumnBtnClicked()
{
    static bool isMute = false;
    static float prevVolume = musicPlayer->getVolume();
    if(!isMute)
    {
        prevVolume = musicPlayer->getVolume();
        musicPlayer->setVolume(0);
        volumeSlider->setValue(0);
        isMute = true;
        volumnBtn->setIcon(QIcon(":/Icon/volumeDisable.png"));
    }
    else
    {
        isMute = false;
        musicPlayer->setVolume(prevVolume);
        volumeSlider->setValue(prevVolume * 100);
        volumnBtn->setIcon(QIcon(":/Icon/volumeLow.png"));
    }


}

