#ifndef VIDEOWINDOW_H
#define VIDEOWINDOW_H

#include <QSlider>
#include <QGraphicsVideoItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QMenuBar>
#include <QStatusBar>
#include <QWidget>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>
#include <QPushButton>
#include <QSoundEffect>
#include "videolist.h"

namespace Ui {
class videowindow;
}
class videowindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit videowindow(QWidget *parent = nullptr);
    ~videowindow();

    void btn_config(QPushButton *btn, QSize size, QString iconpath);
    void url_config(QString path);
    void get_data(QString path);
    void add_data(QString name);
    void data_setting(QTableWidget *table);
    void button_responsive();

signals:
    void nomalscreen();
    void fullscreen();

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void toggleFullScreen(bool fullScreen);
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::videowindow *ui;

    int value_volume, isfull, whetherfavorite;
    bool ishide = true;
    QPushButton *btn_play, *btn_back, *btn_next, *btn_record, *btn_file_op, *btn_last, *btn_volume, *FAQ, *btn_screen_full, *btn_forward;
    QWidget *btn_list , *record_list;
    QMediaPlayer *player1;
    QMediaPlayer *player2;
    QSlider *slider_volume, *slider_video;
    QMediaPlayer *play_area;
    QMediaPlaylist *video_bar, *list_bar;
    QVideoWidget *widget_item;
    QString filepath;
    QTimer *updataslider;
    QLabel *time, *list_name, *bordervideo, *bordervideopath;
    QVector<QString> array_url;
    QPropertyAnimation *Animation_setting;
    video_list *item_list, *list;
    QMenu *background_list, *speed_list, *speed_list_item,*language_list;
    QMenuBar *menu_bar;
    QAction *chinese, *english, *custom, *like, *halfspeed, *sevenfivespeed, *onespeed,*onefivespeed, *twospeed, *picture;


    void init_upper_border();
    void upper_menu();
    void upper_menu_connections();
    void processSelectedDirectory(const QString &directory);
    void Volumeadjust(QString path);
    void border_bar_init();
    void initTimeLabel();
    void initBorderVideoLabels();
    void init_widget();
    void initwindow();
    void initializePlaylists();
    void initializeMediaPlayer();
    void initializeWidget();
    void updateWidgetVisibility();
    void createRecordList();
    void connectCellDoubleClick(int i, int j);
    void list_init();
    void handleCellDoubleClick(int i, int j);
    int findMediaIndex(const QString &path);
    void addMediaAndSetCurrentIndex(const QString &path);
    void updateStatusBar(const QString &path);
    list_widget *createListWidget(const QString &name, int row);
    void connectItemSignals(list_widget *item);
    void get_url(const QString &directory);
    int findMediaI(QMediaPlaylist *playlist, const QString &path, bool &hasFile);

    enum FavoriteStatus {
        NotFavorite,
        Favorite
    };

    FavoriteStatus favoriteStatus = NotFavorite;

};

#endif // VIDEOWINDOW_H
