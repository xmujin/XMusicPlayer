#ifndef MUSICLISTITEM_H
#define MUSICLISTITEM_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
class MusicListItem : public QWidget
{
    Q_OBJECT
public:
    explicit MusicListItem(QWidget *parent = nullptr);
    explicit MusicListItem(const QString& musicName, const QPixmap& musicImg, QWidget *parent = nullptr);



protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;



private:
    QWidget* coverContainer; // 封面容器  包含图片和按钮
    QLabel *musicNameLabel;
    QPushButton *playBtn;



signals:
};

#endif // MUSICLISTITEM_H
