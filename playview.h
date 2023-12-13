#ifndef PLAYVIEW_H
#define PLAYVIEW_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>
#include <QPushButton>
#include <QSlider>
#include <QGraphicsVideoItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QPropertyAnimation>
#include <QMouseEvent>
#include "mytable.h"
#include "tableitem.h"

namespace Ui {
class playView;
}

class playView : public QWidget
{
    Q_OBJECT

public:
    explicit playView(QWidget *parent = nullptr);
    ~playView();
    void initUI();
    void initplayer();
    void initMovewidget();
    void initbtnicon(QPushButton *btn,QSize size,QString iconpath);
    void setsrc(QString path);

    void insertdateTotable(QString name);
    void searchMp4Files(const QString& directory);
    void setdataTotable(QTableWidget *table);
    void resizeBtn();

protected:
    void resizeEvent(QResizeEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::playView *ui;

private:
    QMediaPlayer *myplayer = nullptr;           //播放者
    QMediaPlaylist *myplayerlist = nullptr;     //播放列表
    QVideoWidget *myvideowidget = nullptr;           //在哪播放
    QString filepath;
    QTimer *updataslider = nullptr;

private:
    QWidget *controlwidget = nullptr; //控制中心
    QPushButton *open = nullptr;
    QPushButton *stop_start = nullptr;
    QPushButton *back = nullptr;
    QPushButton *advance = nullptr;
    QPushButton *voice = nullptr;
    QPushButton *localbtn = nullptr;
    QPushButton *howplay = nullptr;
    QSlider *voiceslider = nullptr;
    QSlider *playslider = nullptr;
    int voicevalue;
    QVector<QString> singer;

    //滑动播放队列
    QWidget *movewidget;
    QPropertyAnimation *m_propertyAnimation;
    bool ishide = true;
    mytable *listtable;

signals:
    void nomalscreen();
    void fullscreen();
};

#endif // PLAYVIEW_H
