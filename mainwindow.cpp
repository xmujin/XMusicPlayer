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



    QWidget *centralWidget = new QWidget(this);
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

    musicList = new QListWidget(this);
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

    m_player = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput;
    m_player->setAudioOutput(m_audioOutput);
    m_audioOutput->setVolume(0.1);
    m_musicDir = "D:\\xiangxun\\Music\\";
    loadMusicDir(m_musicDir);
    musicList->setCurrentRow(0);
    startMusic();


}

MainWindow::~MainWindow() {}




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

    connect(playBtn, &QPushButton::clicked, this, &MainWindow::handlePlaySlot);
    connect(modeBtn, &QPushButton::clicked, this, &MainWindow::handleModeSlot);
    connect(nextBtn, &QPushButton::clicked, this, &MainWindow::handleNextSlot);
    connect(listBtn, &QPushButton::clicked, this, &MainWindow::handleShowListSlot);

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



    QFileInfoList fileList = dir.entryInfoList(QDir::Files);
    for(auto element : fileList)
    {
        if(element.suffix() == "mp3")
        {
            // 创建ListWidget的item项
            QListWidgetItem *item = new QListWidgetItem(musicList);
            item->setSizeHint(QSize(0, 80));
            item->setText(element.baseName());
            QPixmap pixmap(":/Icon/音符.png");
            MusicListItem* dsb = new MusicListItem(element.baseName(), pixmap, this);



            // 异步获取封面
            // QMediaPlayer *player = new QMediaPlayer(this);
            // player->setSource(QUrl::fromLocalFile(element.filePath()));
            // connect(player, &QMediaPlayer::metaDataChanged, this, [=]() {
            //     QVariant coverVariant = player->metaData().value(QMediaMetaData::ThumbnailImage);
            //     if(coverVariant.canConvert<QImage>()) {
            //         QPixmap pix = QPixmap::fromImage(coverVariant.value<QImage>());
            //         img->setPixmap(pix.scaled(img->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            //     }
            //     else
            //     {
            //         QPixmap pixmap(":/Icon/音符.png");
            //         img->setPixmap(pixmap.scaled(img->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            //     }
            // });


            musicList->setItemWidget(item, dsb);


        }
    }

    // 连接信号和槽
    // connect(musicList, &QListWidget::itemSelectionChanged, this, [=]() {
    //     for (int i = 0; i < musicList->count(); ++i) {
    //         QListWidgetItem *it = musicList->item(i);
    //         QWidget *w = musicList->itemWidget(it);

    //         if (it->isSelected()) {
    //             w->setStyleSheet(R"(
    //             QWidget {
    //                 background: #edeeef;
    //                 border-radius: 0px;

    //             }

    //             QWidget:hover {
    //                 background: #edeeef;
    //             }
    //             )");
    //         } else {
    //             w->setStyleSheet(R"(
    //             QWidget {
    //                 background: white;
    //                 border-radius: 0px;

    //             }

    //             QWidget:hover {
    //                 background: #edeeef;
    //             }
    //             )");
    //         }
    //     }
    // });
}

void MainWindow::startMusic()
{
    QListWidgetItem *item = musicList->currentItem();

    QString musicName = m_musicDir + item->text() + ".mp3";
    m_player->setSource(QUrl::fromLocalFile(musicName));
}

/**
 * @brief MainWindow::handlePlaySlot 处理音乐暂停与播放
 */
void MainWindow::handlePlaySlot()
{
    if(m_player->playbackState() == QMediaPlayer::PlayingState)
    {
        m_player->pause();
        playBtn->setIcon(QIcon(":/Icon/play.png"));
    }
    else
    {
        m_player->play();
        playBtn->setIcon(QIcon(":/Icon/pause.png"));
    }

}


void MainWindow::handleNextSlot()
{
    int currentRow = musicList->currentRow();
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

    musicList->setCurrentRow(nextRow);
    startMusic();
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

