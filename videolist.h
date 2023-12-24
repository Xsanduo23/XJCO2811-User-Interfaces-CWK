#ifndef VIDEOLIST_H
#define VIDEOLIST_H

#include <QTableWidget>
#include <QWidget>
#include <QIcon>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>



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
class list_widget : public QWidget
{
    Q_OBJECT

public:
    explicit list_widget(QWidget *parent = nullptr);
    ~list_widget();
    // Get text content
    QString txt_catch();
    // Set text content
    void txt_setting(QString text);
    // Set icon
    void icon_setting(QIcon icon);
    // Set the size of the favorite icon
    void favorite_setting(QSize size);
    // Toggle like status
    void toggleLike();
    // Play video
    void playVideo();
    // Get the X coordinate of the label
    int Label_x() const;
    // Set the X coordinate of the label
    void x_setting(int X);

    // Get the Y coordinate of the label
    int Label_y() const;
    // Set the Y coordinate of the label
    void y_setting(int Y);

    // Check if liked
    void is_like(QString name);
    // Set the name
    void name_setting(QString thisname);

protected:
    // Handle resize events
    void resizeEvent(QResizeEvent* event) override;
    // Event filter
    bool eventFilter(QObject *object, QEvent *event) override;
    // Mouse enter event
    void enterEvent(QEvent *event) override;
    // Mouse leave event
    void leaveEvent(QEvent *event) override;

signals:
    // Emit signal to add to favorites
    void favorite_add();
    // Emit signal to cancel favorites
    void favorite_cancel();
    // Emit signal to play the video
    void video_play();

private:
    Ui::list_widget *ui;
    // Initialize the window
    void initwindow();

    int x, y;
    QIcon myicon;

    // Mobile screen
    QMediaPlayer *play_area = nullptr;           // Player
    QMediaPlaylist *video_bar = nullptr;     // Playlist
    QVideoWidget *widget_item = nullptr;           // Where to play the video
    QString filepath;
};

#endif // VIDEOLIST_H
