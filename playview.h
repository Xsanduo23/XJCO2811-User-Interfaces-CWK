#ifndef PLAYVIEW_H
#define PLAYVIEW_H

#include <QWidget>
#include <QMainWindow>
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
#include <QMenuBar>
#include <QStatusBar>
#include <QSoundEffect>
#include "mytable.h"
#include "tableitem.h"

namespace Ui {
class playView;
}

class playView : public QMainWindow
{
    Q_OBJECT

public:
    explicit playView(QWidget *parent = nullptr);
    ~playView();
    void initUI();
    void initplayer();
    void initMovewidget();
    void initList();
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
    QPushButton *fullbtn = nullptr;
    QPushButton *stop_start = nullptr;
    QPushButton *backlitle = nullptr;
    QPushButton *advancelitle = nullptr;
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

    //列表
    mytable *table;

    //状态栏
    void initStatebar(); //设置状态栏
    QLabel *localvideo;

signals:
    void nomalscreen();
    void fullscreen();

private:
    QMenuBar *menu;
    QMenu *menu2;
    QMenu *menu3;
    QMenu *menu4;
    QAction *action1;
    QAction *action2;
    QAction *action3;
    QAction *like;
    QAction *onespeed;
    QAction *twospeed;
    QAction *zerofivespeed;
    int islikeicon = 0;
    QAction *photo;
    QStatusBar *status;
    void initMenu();

    void playSound(QString url);

};

#endif // PLAYVIEW_H
