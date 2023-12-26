#ifndef VIDEOLIST_H
#define VIDEOLIST_H

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QTableWidget>
#include <QWidget>
#include <QVideoWidget>
#include <QIcon>


class video_list : public QTableWidget
{
    Q_OBJECT
public:
    // Set the table title
    void setTitle(QString header);
    explicit video_list(QWidget *parent = nullptr);
    // Initialize table settings
    void initializeTableSettings();
    // Set table styles
    void setTableStyles();
    // Load style sheet
    void loadStyleSheet(const QString &styleSheetFile);
signals:

};

namespace Ui {
class list_widget;
}

class list_widget : public QWidget {
    Q_OBJECT

public:
    explicit list_widget(QWidget *parent = nullptr);
    ~list_widget();

    // Getters
    int Label_x() const;
    int Label_y() const;

     // Setters
    void icon_setting(QIcon icon);
    void favorite_setting(QSize size);
    void x_setting(int X);
    void y_setting(int Y);
    void name_setting(QString thisname);
    void is_like(QString name);
    QString txt_catch();
    void txt_setting(QString text);



protected:
    // Event Handlers
    bool eventFilter(QObject *object, QEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void resizeEvent(QResizeEvent* event) override;


signals:
    // Signals
    void favorite_add();
    void favorite_cancel();
    void video_play();

private:
    Ui::list_widget *ui;

    // Private Members
    int x;
    int y;
    QIcon myicon;

    // Media Components
    QVideoWidget *widget_item;      // Video Widget
    QMediaPlayer *play_area;        // Player
    QMediaPlaylist *video_bar;      // Playlist
    QString filepath;

    // Actions
    void toggleLike();
    void playVideo();
    void initwindow();
};


#endif // VIDEOLIST_H
