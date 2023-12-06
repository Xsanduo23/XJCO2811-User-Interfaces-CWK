#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>
#include <QFileDialog>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void setButtonImage(QPushButton *button, QString image,int xsize,int ysize);
    int boolShow;
    void AllHide(); //全屏时，部分控件隐藏
    void AllShow();

private slots:
    void on_pushButton_start_stop_clicked();
    void on_pushButton_choice_clicked();
    void onTimeOut();   //设置定时器，实时同步屏幕大小
    void onTimeOutBar();  //实现时评进度条实时更新
    void ChangedVoice(int); //改变声音大小
//    void ChangedVoiceBar(int);
    void on_pushButton_sound_clicked();

    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_sliderReleased();

    void on_pushButton_kuaijin_clicked();

    void on_pushButton_houtui_clicked();

private:
    Ui::Widget *ui;
    QMediaPlayer *myplayer = nullptr;           //播放者
    QMediaPlaylist *myplayerlist = nullptr;     //播放列表
    QVideoWidget *mywidget = nullptr;           //在哪播放
    bool start;
    bool sound;
    QTimer my_timer;   //定时器，调整屏幕大小
    QTimer playbar;    //定时器，调整视频进度条

    void enterEvent(QEvent *e);                      //进入QWidget瞬间事件

    void leaveEvent(QEvent *e);                      //离开QWidget瞬间事件

};
#endif // WIDGET_H
