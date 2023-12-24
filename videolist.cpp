#include "videolist.h"
#include "ui_videolist.h"
#include <QDebug>
#include <QTimer>
#include <QFile>
#include <QHeaderView>
video_list::video_list(QWidget *parent) : QTableWidget(parent) {
    //Constructor for the 'video_list' class.
    //This constructor initializes an instance of the 'video_list' class with the given 'parent' widget.
    //It also calls the 'initializeTableSettings' method to set up the table's initial settings.
    //Parameters:
    //- parent: A pointer to the parent widget (usually the main application window).
    //Note: The 'initializeTableSettings' method configures various table properties and styles.
    initializeTableSettings();
    setStyleSheet("QHeaderView"
                        "{"
                        "background:transparent;"
                        "}"
                        "QHeaderView::section"
                        "{"
                        "font-size:14px;"
                        "font-family:\"Microsoft YaHei\";"
                        "color:#FFFFFF;"

                        "background:#60669B;"
                        "border:none;"
                        "text-align:left;"

                        "min-height:49px;"
                        "max-height:49px;"

                        "margin-left:0px;"
                        "padding-left:0px;"
                        "}"
                        "QTableWidget"
                        "{"
                        "background:transparent;"
                        "border:none;"

                        "font-size:16px;"
                        "font-family:\"Microsoft YaHei\";"
                        "color:white;"
                        "}"
                        "QTableWidget::item"
                        "{"
                        "border:none;"
                        "}"
                        "QTableWidget::item::selected"
                        "{"
                        "color:red;"
                        "background:rgb(36,36,38);"
                        "}"

                        "QTableView::item:hover"
                        "{"
                        "background-color:rgb(36,36,38);"
                        "}"

                        "QScrollBar::handle:vertical"
                        "{"
                        "background: rgba(255,255,255,50%);"
                        "border: 0px solid grey;"
                        "border-radius:3px;"
                        "width: 8px;"
                        "}"
                        "QScrollBar::vertical"
                        "{"
                        "border-width:1px;"
                        "border-style: solid;"
                        "border-color: rgba(255, 255, 255, 10%);"
                        "width: 8px;"
                        "margin:0px 0px 0px 0px;"
                        "border-radius:3px;"
                        "}"
                        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical"
                        "{"
                        "background:rgba(255,255,255,10%);"
                        "}"
                        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical"
                        "{"
                        "background:transparent;"
                        "}"
                        );

}

void video_list::initializeTableSettings() {
    // Initialize table settings for the 'video_list' class.
    // This method configures various properties of the table, including column count,
    // header labels, selection behavior, resizing mode, minimum size, visibility of vertical header,
    // context menu policy, and mouse tracking.
    setColumnCount(1);
    setHorizontalHeaderLabels(QStringList() << "Play List");
    setSelectionBehavior(QAbstractItemView::SelectRows);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    setMinimumSize(10, 10);
    verticalHeader()->setVisible(false);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setMouseTracking(true);
    setTableStyles();
}


void video_list::setTableStyles() {
    // Set table styles for the 'video_list' class.
    // This method sets the stylesheet for the table to have white text color.
    // It also customizes the stylesheet for the horizontal header with a transparent background
    // and red text color.
    setStyleSheet("color: white");
    horizontalHeader()->setStyleSheet("background-color: transparent; color: red;");
}



void video_list::setTitle(QString headername) {
    QStringList headers;
    headers<<headername;
    this->setHorizontalHeaderLabels(headers);
}
list_widget::list_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::list_widget)
{
    ui->setupUi(this);
    initwindow();
    this->setStyleSheet("background-color: transparent;");
    ui->list_play_ui->hide();
    ui->favorite_btn_ui->installEventFilter(this);
    ui->favorite_btn_ui->setObjectName("unlike");

    connect(ui->favorite_btn_ui, &QToolButton::clicked, this, &list_widget::toggleLike);
    connect(ui->list_play_ui, &QToolButton::clicked, this, &list_widget::playVideo);
}
void list_widget::enterEvent(QEvent *event)
{
    // Enter event handler for the 'list_widget' class.
    // This method is called when the mouse cursor enters the widget.
    // It checks the object name of the widget and shows the 'list_play_ui' if it matches "movelisttableitem".
    // Additionally, it starts playing the video using the 'play_area' when the cursor enters the widget.

    // Check if the object name of the widget is "movelisttableitem"
    if(this->objectName()=="movelisttableitem")
    {
        // Show the 'list_play_ui'
        ui->list_play_ui->show();
    }
    else
    {
        // Show the 'list_play_ui'
        ui->list_play_ui->show();
    }

    // Start playing the video using the 'play_area'
    play_area->play();
}
void list_widget::toggleLike()
{
    // Toggle the like status for the 'list_widget' class.
    // This method is responsible for toggling the like status of the widget.
    // If the current state is not "like," it sets the icon to a filled heart, changes the object name,
    // and emits the 'favorite_add' signal to indicate that the video should be added to favorites.
    // If the current state is "like," it sets the icon to an empty heart, changes the object name,
    // and emits the 'favorite_cancel' signal to indicate that the video should be removed from favorites.

    // Check if the current object name is not "like"
    if (ui->favorite_btn_ui->objectName() != "like")
    {
        // Set the icon to a filled heart
        ui->favorite_btn_ui->setIcon(QIcon(":/img/aixin2.png"));
        // Change the object name to "like"
        ui->favorite_btn_ui->setObjectName("like");
        // Emit the 'favorite_add' signal
        emit favorite_add();
    }
    else
    {
        // Set the icon to an empty heart
        ui->favorite_btn_ui->setIcon(QIcon(":/img/aixin.png"));
        // Change the object name to "unlike"
        ui->favorite_btn_ui->setObjectName("unlike");
        // Emit the 'favorite_cancel' signal
        emit favorite_cancel();
    }
}

void list_widget::playVideo()
{
    emit video_play();
}


bool list_widget::eventFilter(QObject *object, QEvent *event)
{
    // Event filter for the 'list_widget' class.
    // This method filters and handles events for the 'favorite_btn_ui' widget.
    // When the mouse enters the button area, it performs scaling by changing the icon and size.
    // When the mouse leaves the button area, it restores the original icon and size.

    // Check if the object is the 'favorite_btn_ui'
    if (object == ui->favorite_btn_ui)
    {
        // Check if the event type is "Enter" (mouse enters the button area)
        if (event->type() == QEvent::Enter)
        {
            // Mouse enters the button area, perform scaling
            if (ui->favorite_btn_ui->objectName() != "like")
            {
                // Change the icon to a filled heart
                ui->favorite_btn_ui->setIcon(QIcon(":/img/aixin1.png"));
            }
            // Set the new icon size
            ui->favorite_btn_ui->setIconSize(QSize(30, 30));
        }
        // Check if the event type is "Leave" (mouse leaves the button area)
        else if (event->type() == QEvent::Leave)
        {
            if (ui->favorite_btn_ui->objectName() != "like")
            {
                // Change the icon to an empty heart
                ui->favorite_btn_ui->setIcon(QIcon(":/img/aixin.png"));
            }
            else
            {
                // Change the icon to a filled heart
                ui->favorite_btn_ui->setIcon(QIcon(":/img/aixin2.png"));
            }
            // Mouse leaves the button area, restore the original size
            ui->favorite_btn_ui->setIconSize(QSize(20, 20));
        }
    }
    // Pass the event to the base class
    return QWidget::eventFilter(object, event);
}
void list_widget::leaveEvent(QEvent *event)
{
    // Leave event handler for the 'list_widget' class.
    // This method is called when the mouse cursor leaves the widget.
    // It checks the object name of the widget and hides the 'list_play_ui' if it matches "movelisttableitem".
    // Additionally, it pauses the video playback using the 'play_area' when the cursor leaves the widget.

    // Check if the object name of the widget is "movelisttableitem"
    if(this->objectName()=="movelisttableitem")
    {
        // Hide the 'list_play_ui'
        ui->list_play_ui->hide();
    }
    else
    {
        // Hide the 'list_play_ui'
        ui->list_play_ui->hide();
    }

    // Pause the video playback using the 'play_area'
    play_area->pause();
}

void list_widget::resizeEvent(QResizeEvent *event)
{
    widget_item->resize(ui->widget_show->size());
    widget_item->update();
}
void list_widget::is_like(QString name)
{
    ui->favorite_btn_ui->setObjectName(name);
}
void list_widget::name_setting(QString thisname)
{
    this->setObjectName(thisname);
}

list_widget::~list_widget()
{
    delete ui;
}

QString list_widget::txt_catch()
{
    return ui->list_info_ui->text();
}
void list_widget::txt_setting(QString text)
{
    // Set text and video URL for the 'list_widget' class.
    // This method sets the text for the 'label' widget, sets the media URL for the 'play_area' (video player),
    // and initiates playback. It also calculates a new position for the video and sets it before pausing.

    // Set the text of the 'label' widget
    ui->list_info_ui->setText(text);

    // Set the media URL for the 'play_area' (video player)
    play_area->setMedia(QUrl(text));

    // Play the video
    play_area->play();

    // Calculate a new position for the video (10% of the video duration)
    qint64 newPosition = 0.1 * play_area->duration();

    // Pause the video playback
    play_area->pause();

    // Set the calculated position for the video
    play_area->setPosition(newPosition);
}

void list_widget::icon_setting(QIcon icon)
{
    ui->favorite_btn_ui->setIcon(icon);
    myicon = icon;
}

void list_widget::favorite_setting(QSize size)
{
    ui->favorite_btn_ui->setIconSize(size);
}
void list_widget::initwindow()
{
    // Initialize the window and media components for the 'list_widget' class.
    // This method initializes the video player and playlist components.
    // It also sets the playback mode of the playlist to loop.
    // Additionally, it configures the video widget to display video output.

    // Create a new QMediaPlayer instance
    play_area = new QMediaPlayer();

    // Create a new QMediaPlaylist instance
    video_bar = new QMediaPlaylist();

    // Set the playback mode of the playlist to loop
    video_bar->setPlaybackMode(QMediaPlaylist::Loop);

    // Create a new QVideoWidget instance within the 'widget_show'
    widget_item = new QVideoWidget(ui->widget_show);

    // Set the playlist for the QMediaPlayer
    play_area->setPlaylist(video_bar);

    // Set the video output to the QVideoWidget
    play_area->setVideoOutput(widget_item);

    // Resize the video widget to match the size of 'widget_show'
    widget_item->resize(ui->widget_show->size());

    // Set the volume of the media player to 0 (mute)
    play_area->setVolume(0);
}



int list_widget::Label_y() const
{
    return y;
}

void list_widget::y_setting(int Y)
{
    y = Y;
}

int list_widget::Label_x() const
{
    return x;
}

void list_widget::x_setting(int X)
{
    x = X;
}
