#ifndef VIDEOWINDOW_H
#define VIDEOWINDOW_H

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

    void init_widget();
    void initwindow();
    void initializePlaylists();
    void initializeMediaPlayer();
    void initializeWidget();
    void updateWidgetVisibility();
    void createRecordList(); // Create the window for the video list
    void connectCellDoubleClick(int i, int j);
    void list_init();
    void handleCellDoubleClick(int i, int j);

    void btn_config(QPushButton *btn, QSize size, QString iconpath);

    void url_config(QString path);

    void get_data(QString path);
    int findMediaIndex(const QString &path);
    void addMediaAndSetCurrentIndex(const QString &path);
    void updateStatusBar(const QString &path);
    void add_data(QString name);
    list_widget *createListWidget(const QString &name, int row);
    void connectItemSignals(list_widget *item);
    void get_url(const QString &directory);

    void data_setting(QTableWidget *table);
    void button_responsive();
    int findMediaI(QMediaPlaylist *playlist, const QString &path, bool &hasFile);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void toggleFullScreen(bool fullScreen);

    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::videowindow *ui;

    QPushButton *btn_play = nullptr;
    QPushButton *btn_back = nullptr;
    QPushButton *btn_next = nullptr;
    QPushButton *btn_record = nullptr;
    QPushButton *btn_file_op = nullptr;
    QPushButton *btn_last = nullptr;
    QPushButton *btn_volume = nullptr;
    QPushButton *FAQ = nullptr;
    QPushButton *btn_screen_full = nullptr;
    QPushButton *btn_forward = nullptr;
    QWidget *btn_list = nullptr; // Control center
    QSlider *slider_volume = nullptr;
    QSlider *slider_video = nullptr;
    QMediaPlayer *play_area = nullptr;       // Player
    QMediaPlaylist *video_bar = nullptr;     // Playlist
    QMediaPlaylist *list_bar = nullptr;      // Playlist
    QVideoWidget *widget_item = nullptr;     // Where to play the video
    QString filepath;
    QTimer *updataslider = nullptr;
    QLabel *time;
    int value_volume;
    int isfull = 0;
    QVector<QString> array_url;

    // Status bar
    void border_bar_init(); // Set the status bar
    void initTimeLabel();
    void initBorderVideoLabels();
    QLabel *bordervideo;
    QLabel *bordervideopath;
    // Scroll through the playlist
    QWidget *record_list;
    QPropertyAnimation *Animation_setting;
    video_list *item_list;
    QLabel *list_name;
    bool ishide = true;
    video_list *list;


    QMenu *background_list;
    QMenu *speed_list;
    QMenu *speed_list_item;
    QMenuBar *menu_bar;
    QMenu *language_list;

    QAction *chinese;
    QAction *english;
    QAction *custom;
    QAction *like;
    QAction *halfspeed;
    QAction *onespeed;
    QAction *twospeed;
    int whetherfavorite = 0;
    QAction *picture;
    void init_upper_border();
    void upper_menu();
    void upper_menu_connections();
    void processSelectedDirectory(const QString &directory);
    void Volumeadjust(QString path);

    enum FavoriteStatus {
        NotFavorite,
        Favorite
    };


    FavoriteStatus favoriteStatus = NotFavorite;

signals:
    void nomalscreen();
    void fullscreen();


};

#endif // VIDEOWINDOW_H
