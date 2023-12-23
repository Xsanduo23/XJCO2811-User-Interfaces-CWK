#include "videowindow.h"
#include "ui_videowindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QTimer>
#include <qDebug>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QColorDialog>
#include <QDateTime>
int i =0;

// Constructor for the 'videowindow' class.
// This constructor initializes an instance of the 'videowindow' class with the given 'parent' widget.
// It sets up the user interface, initializes playback devices and widgets, and configures timers and initial settings.

videowindow::videowindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::videowindow)
{
    ui->setupUi(this);

    // Initialize playback devices and widgets
    initwindow();

    // Initialize the user interface
    init_widget();

    // Create a timer to update the playback progress and time display
    updataslider = new QTimer();
    connect(updataslider, &QTimer::timeout, this, [=]() {
        try {
            if (play_area->duration()) {
                // Calculate the position and duration time
                QTime positionTime(0, 0);
                positionTime = positionTime.addMSecs(play_area->position());
                QTime durationTime(0, 0);
                durationTime = durationTime.addMSecs(play_area->duration());

                // Update the time display
                time->setText(QString("%1/%2").arg(positionTime.toString("mm:ss"), durationTime.toString("mm:ss")));

                // Update the progress bar
                slider_video->setValue(int(play_area->position() * 500 / play_area->duration()));
            }
        } catch (const std::exception& e) {
            // Handle exceptions, e.g., output error messages or perform error handling
        }
    });

    // Set the fixed height for 'widget_2'
    ui->widget_2->setFixedHeight(120);

    // Disable dragging of the initial progress bar
    slider_video->setEnabled(false);

    // Set the initial volume level
    value_volume = 20;

    // Create a timer to adjust volume with a sound effect
    QTimer *timer = new QTimer();
    connect(timer, &QTimer::timeout, this, [=]() {
        this->resize(this->width(), this->height() + 1);
        i++;
        if (i > 3) {
            // Adjust the volume and stop the timer
            Volumeadjust(":/music/Windows open.wav");
            timer->stop();
        }
    });
    timer->start(10);

    // Initialize upper menu and border
    init_upper_border();
    border_bar_init();

    // Set the window icon
    this->setWindowIcon(QIcon(":/img/logo.png"));

    // Set the initial window position
    this->move(500, 5);
}

videowindow::~videowindow()
{
    delete ui;
}

void videowindow::init_widget()
{
    btn_list = new QWidget(ui->widget_2);
    btn_list->resize(ui->widget_2->size());

    time = new QLabel("00:00/00:00");
    time->setStyleSheet("color:white");

    btn_screen_full = new QPushButton();
    btn_config(btn_screen_full,QSize(this->width()*0.04,this->width()*0.04),":/img/fullscreen.png");
    btn_file_op = new QPushButton();
    btn_config(btn_file_op,QSize(this->width()*0.05,this->width()*0.05),":/img/wenjianjia.png");
    btn_back = new QPushButton();
    btn_config(btn_back,QSize(this->width()*0.04,this->width()*0.04),":/img/backward-full.png");
    btn_forward = new QPushButton();
    btn_config(btn_forward,QSize(this->width()*0.04,this->width()*0.04),":/img/go-full.png");

    btn_play = new QPushButton();
    btn_config(btn_play,QSize(this->width()*0.039,this->width()*0.039),":/img/zanting.png");
    space = new QPushButton("");
    btn_config(space,QSize(this->width()*0.07,this->width()*0.07),"");
    space->setStyleSheet("");
    btn_last = new QPushButton();
    btn_config(btn_last,QSize(this->width()*0.06,this->width()*0.06),":/img/prev-full.png");
    btn_next = new QPushButton();
    btn_config(btn_next,QSize(this->width()*0.06,this->width()*0.06),":/img/next-full.png");
    btn_volume = new QPushButton();
    btn_volume->setObjectName("yes");
    btn_config(btn_volume,QSize(this->width()*0.05,this->width()*0.05),":/img/24gf-volumeHigh.png");
    btn_record = new QPushButton();
    btn_record->setObjectName("yes");
    btn_config(btn_record,QSize(this->width()*0.05,this->width()*0.05),":/img/24gl-playlistVideo.png");

    slider_volume = new QSlider(this);
    slider_volume->resize(15,10);
    slider_volume->setMaximum(100);
    slider_volume->setPageStep(10);
    slider_volume->setOrientation(Qt::Horizontal);
    slider_volume->setValue(20);
    slider_volume->setMinimumWidth(70);
    slider_volume->setMaximumWidth(100);

    QHBoxLayout *hlayout = new QHBoxLayout();
    QSpacerItem *spacer1 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    QSpacerItem *spacer2 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);


    hlayout->addWidget(btn_file_op);
    hlayout->addWidget(space);
    hlayout->addSpacerItem(spacer1);
    hlayout->addWidget(btn_back);
    hlayout->addWidget(btn_last);
    hlayout->addWidget(btn_play);
    hlayout->addWidget(btn_next);
    hlayout->addWidget(btn_forward);
    hlayout->addWidget(btn_screen_full);
    hlayout->addSpacerItem(spacer2);
    hlayout->addWidget(btn_volume);
    hlayout->addWidget(slider_volume);
    hlayout->addWidget(btn_record);
    hlayout->setStretch(0, 1);
    hlayout->setStretch(1, 2);
    hlayout->setStretch(2, 7);
    hlayout->setStretch(3, 1);
    hlayout->setStretch(4, 1);
    hlayout->setStretch(5, 1);
    hlayout->setStretch(6, 1);
    hlayout->setStretch(7, 1);
    hlayout->setStretch(8, 1);
    hlayout->setStretch(9, 6);
    hlayout->setStretch(10, 1);
    hlayout->setStretch(11, 3);
    hlayout->setStretch(12,1);
    hlayout->setSpacing(0);

    QVBoxLayout *vlayout = new QVBoxLayout(btn_list);

    slider_video = new QSlider();
    slider_video->setMaximum(500);
    slider_video->setPageStep(10);
    slider_video->setOrientation(Qt::Horizontal);

    slider_video = new QSlider();
    slider_video->setMaximum(500);
    slider_video->setPageStep(10);
    slider_video->setOrientation(Qt::Horizontal);


    slider_video->setStyleSheet(        "QSlider::sub-page:horizontal {"
                                "    margin-bottom: 8px;"
                                "    background: rgb(90, 49, 255);"
                                "    margin-top: 8px;"
                                "    border-radius: 2px;"
                                "}"
                                "QSlider::add-page:horizontal {"
                                "    margin-top: 9px;"
                                "    border-radius: 2px;"
                                "    margin-bottom: 9px;"
                                "    background: rgb(255, 255, 255);"
                                "    border: 0px solid #777;"
                                "}"
                                "QSlider::handle:horizontal {"
                                "    border: 1px solid rgb(193, 204, 208);"
                                "    margin-top: 6px;"
                                "    margin-bottom: 6px;"
                                "    border-radius: 2px;"
                                "    width: 5px;"
                                "    background: rgb(193, 204, 208);"
                                "}"
                                "QSlider::groove:horizontal {"
                                "    border: 0px solid #bbb;"
                                "}"
                                "QSlider::handle:horizontal:hover {"
                                "    margin-top: 4px;"
                                "    border-radius: 5px;"
                                "    width: 10px;"
                                "    border: 1px solid rgb(193, 204, 208);"
                                "    margin-bottom: 4px;"
                                "    background: rgb(193, 204, 208);"
                                "}");
    slider_volume->setStyleSheet(        "QSlider::sub-page:horizontal {"
                                 "    margin-bottom: 8px;"
                                 "    background: rgb(90, 49, 255);"
                                 "    margin-top: 8px;"
                                 "    border-radius: 2px;"
                                 "}"
                                 "QSlider::add-page:horizontal {"
                                 "    margin-top: 9px;"
                                 "    border-radius: 2px;"
                                 "    margin-bottom: 9px;"
                                 "    background: rgb(255, 255, 255);"
                                 "    border: 0px solid #777;"
                                 "}"
                                 "QSlider::handle:horizontal {"
                                 "    border: 1px solid rgb(193, 204, 208);"
                                 "    margin-top: 6px;"
                                 "    margin-bottom: 6px;"
                                 "    border-radius: 2px;"
                                 "    width: 5px;"
                                 "    background: rgb(193, 204, 208);"
                                 "}"
                                 "QSlider::groove:horizontal {"
                                 "    border: 0px solid #bbb;"
                                 "}"
                                 "QSlider::handle:horizontal:hover {"
                                 "    margin-top: 4px;"
                                 "    border-radius: 5px;"
                                 "    width: 10px;"
                                 "    border: 1px solid rgb(193, 204, 208);"
                                 "    margin-bottom: 4px;"
                                 "    background: rgb(193, 204, 208);"
                                 "}");

    QHBoxLayout *hlayout1 = new QHBoxLayout();
    hlayout1->addWidget(time);
    hlayout1->addWidget(slider_video);

    vlayout->addLayout(hlayout1);
    vlayout->addLayout(hlayout);

    btn_list->resize(ui->widget_2->size());
    createRecordList();
    list_init();
    btn_list->show();

    // Connect a slot to the 'clicked' signal of the 'btn_file_op' button.
    // This connects a slot to the button click event, allowing the user to select a directory.
    // The selected directory is then processed to obtain a list of URLs.

    connect(btn_file_op, &QPushButton::clicked, this, [=]() {
        // Clear the array_url to store new URLs
        array_url.clear();

        // Open a file dialog to select a directory
        QString directory = QFileDialog::getExistingDirectory(
            nullptr,
            "Select Folder",
            QDir::homePath(),
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

        if (!directory.isEmpty()) {
            qDebug() << "Selected directory path: " << directory;
            try {
                // Get URLs from the selected directory
                get_url(directory);

                // Configure and display the obtained data using 'list'
                data_setting(list);
            } catch (const std::exception &ex) {
                qDebug() << "Error occurred: " << ex.what();
            }
        }
        qDebug() << array_url;
    });




    // Connect a slot to the 'clicked' signal of the 'btn_play' button.
    // This connection allows toggling the playback state when the button is clicked.
    // If the media player is not in the playing state, it starts playing and updates the button icon.
    // If the media player is already playing, it pauses playback and updates the button icon.

    connect(btn_play, &QPushButton::clicked, this, [=]() {
        if (play_area->state() != QMediaPlayer::PlayingState) {
            // Start playback, update the button icon to 'pause', and log a message
            btn_config(btn_play, QSize(btn_play->iconSize()), ":/img/zanting.png");
            play_area->play();
            qDebug() << "start";
        } else {
            // Pause playback, update the button icon to 'play', and log a message
            btn_config(btn_play, QSize(btn_play->iconSize()), ":/img/bofang.png");
            play_area->pause();
            qDebug() << "stop";
        }
    });

    // Connect slots to the 'sliderMoved' and 'sliderReleased' signals of 'slider_video'.
    // These connections handle user interactions with the video playback progress slider.

    // When the slider is moved, stop the timer for updating playback progress,
    // set the media player's position based on the slider value, and log a message.
    connect(slider_video, &QSlider::sliderMoved, this, [=]() {
        if (updataslider != nullptr) {
            updataslider->stop();
        }
        play_area->setPosition(slider_video->value() * play_area->duration() / 500);
        qDebug() << "Position changed: " << slider_video->value();
    });

    // When the slider is released, restart the timer for updating playback progress
    // and log a message.
    connect(slider_video, &QSlider::sliderReleased, this, [=]() {
        updataslider->start(200);
        qDebug() << "Slider released";
    });

    connect(btn_volume, &QPushButton::clicked, this, [=]() {
        // If the volume button's object name is not "yes"
        if (btn_volume->objectName() != "yes") {
            // Toggle the volume button state to "yes" and set the volume and slider value
            btn_volume->setObjectName("yes"); // Enable volume
            play_area->setVolume(value_volume); // Set volume value
            slider_volume->setValue(value_volume); // Set slider position
            btn_config(btn_volume, QSize(btn_volume->iconSize()), ":/img/24gf-volumeHigh.png"); // Set button icon
            qDebug() << "Volume enabled, value: " << value_volume;
        } else {
            // Toggle the volume button state to "no" and set the slider value to 0, while updating the button icon
            btn_volume->setObjectName("no"); // Disable volume
            btn_config(btn_volume, QSize(btn_volume->iconSize()), ":/img/24gf-volumeDisable.png"); // Set button icon
            slider_volume->setValue(0); // Set slider position to mute
            qDebug() << "Volume disabled";
        }
    });



    // Connect a slot to the 'valueChanged' signal of 'slider_volume'.
    // This connection handles changes in the volume slider's value.

    connect(slider_volume, &QSlider::valueChanged, this, [=](int value) {
        // Set the media player's volume based on the slider value
        play_area->setVolume(value);

        // Log the new volume value
        qDebug() << "New volume: " << value;

        // If the value is not zero, update the 'value_volume' variable
        if (value != 0) {
            value_volume = value;
        }
    });

    // Connect a slot to the 'clicked' signal of the 'btn_record' button.
    // This connection handles the visibility and animation of the record list.

    connect(btn_record, &QPushButton::clicked, this, [=]() {
        if (!ui->widget_4->isVisible()) {
            // Show the record list
            record_list->show();

            // Set animation effect to slide in from the right
            Animation_setting->setStartValue(QRect(this->rect().width(), 0, record_list->width(), record_list->height()));
            Animation_setting->setEndValue(QRect(this->rect().width() - record_list->width(), 0, record_list->width(), record_list->height()));
            Animation_setting->start();

            // Update the state to indicate that the record list is displayed
            ishide = false;

            // Log a debug message
            qDebug() << "Record list is displayed";
        } else {
            // Log a debug message
            qDebug() << "Record list is hidden";

            // Hide the record list
            record_list->hide();

            // Update the state to indicate that the record list is hidden
            ishide = true;
        }
    });


    // Connect a slot to the 'clicked' signal of the 'btn_last' button.
    // This connection handles the functionality for playing the previous media in the playlist.

    connect(btn_last, &QPushButton::clicked, this, [=]() {
        // Stop the timer for updating playback progress
        updataslider->stop();

        // Move to the previous media in the playlist
        list_bar->previous();

        // Get the current media content
        QMediaContent currentMedia = list_bar->currentMedia();

        // Get the file name of the current media
        QString currentPlayerName = currentMedia.canonicalUrl().fileName();

        // Iterate through the list and find the matching item
        for (int i = 0; i < list->rowCount(); i++) {
            QWidget *widget = list->cellWidget(i, 0);
            if (widget) {
                list_widget *item = qobject_cast<list_widget *>(widget);
                if (item) {
                    // Get the text associated with the item
                    QString text = item->txt_catch();

                    // Check if the text contains the current media's file name
                    if (text.contains(currentPlayerName)) {
                        // Retrieve and display the data for the new media
                        get_data(text);
                        break;
                    } else {
                        qDebug() << "No matching item found";
                    }
                }
            } else {
                qDebug() << "No widget found";
            }
        }

        // Start playing the new media
        play_area->play();

        // Restart the timer for updating playback progress
        updataslider->start(200);
    });

    // Connect a slot to the 'clicked' signal of the 'btn_next' button.
    // This connection handles the functionality for playing the next media in the playlist.

    connect(btn_next, &QPushButton::clicked, this, [=]() {
        // Stop the timer for updating playback progress
        updataslider->stop();

        // Move to the next media in the playlist
        list_bar->next();

        // Get the current media content
        QMediaContent currentMedia = list_bar->currentMedia();

        // Get the file name of the current media
        QString currentPlayerName = currentMedia.canonicalUrl().fileName();

        qDebug() << "Filename:" << currentPlayerName;

        // Iterate through the list and find the matching item
        bool found = false;
        for (int i = 0; i < list->rowCount(); i++) {
            QWidget *widget = list->cellWidget(i, 0);
            if (widget) {
                list_widget *item = qobject_cast<list_widget *>(widget);
                if (item) {
                    // Get the text associated with the item
                    QString text = item->txt_catch();

                    // Check if the text contains the current media's file name
                    if (text.contains(currentPlayerName)) {
                        // Execute operations related to the found match
                        get_data(text);
                        found = true;
                        break;
                    }
                }
            } else {
                qDebug() << "No stored widget found";
            }
        }

        // Log a message if no matching item is found
        if (!found) {
            qDebug() << "No matching item found";
        }

        // Start playing the new media
        play_area->play();

        // Restart the timer for updating playback progress
        updataslider->start(200);
    });


    // Connect a slot to the 'clicked' signal of the 'space' button.
    // This connection handles the functionality of toggling the playback mode between playlist loop and current item in loop.

    connect(space, &QPushButton::clicked, this, [=]() {
        if (space->text() == "") {
            // Clear the button text and icon, set the playback mode to playlist loop
            space->setText("");
            btn_config(space, QSize(15, 15), "");
            video_bar->setPlaybackMode(QMediaPlaylist::Loop);

            // Additional code to execute under the 'if' condition
            qDebug() << "Button clicked, executing the 'if' block";
        } else {
            // Clear the button text and icon, set the playback mode to current item in loop
            space->setText("");
            btn_config(space, QSize(15, 15), "");
            video_bar->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);

            // Additional code to execute under the 'else' condition
            qDebug() << "Button clicked, executing the 'else' block";
        }

        // You can add additional operations under each condition as needed
    });


    connect(btn_forward, &QPushButton::clicked, this, [=]() {
        if (play_area->state() == QMediaPlayer::PlayingState) {
            // Check if the media player is in the playing state

            // Calculate the new position for fast-forwarding
            qint64 newPosition = play_area->position() + 0.1 * play_area->duration();

            // Set the new playback position
            play_area->setPosition(newPosition);

            // Additional code, such as displaying a message
            qDebug() << "Fast-forward button clicked, current playback position: " << newPosition;
        }
    });

    // Connect a slot to the 'clicked' signal of the 'btn_back' button.
    // This connection handles the functionality of rewinding the currently playing media.

    connect(btn_back, &QPushButton::clicked, this, [=]() {
        if (play_area->state() == QMediaPlayer::PlayingState) {
            // Check if the playback state is playing

            // Calculate the new position for rewinding
            qint64 newPosition = play_area->position() - 0.1 * play_area->duration();

            // Set the new playback position
            play_area->setPosition(newPosition);

            // Additional code, such as displaying a message
            qDebug() << "Back button clicked, current playback position: " << newPosition;
        }
    });

    // Connect a slot to the 'clicked' signal of the 'btn_screen_full' button.
    // This connection handles the functionality of toggling between full-screen and normal screen modes.

    connect(btn_screen_full, &QPushButton::clicked, this, [=]() {
        if (!isfull) {
            // Enter full-screen mode
            ui->widget_4->hide(); // Hide a specific widget
            this->showFullScreen(); // Show the main window in full-screen mode
            btn_screen_full->setIcon(QIcon(":/img/exitfullscreen.png")); // Set button icon to exit full-screen
            isfull = 1; // Update the full-screen state flag
        } else {
            // Exit full-screen mode
            this->showNormal(); // Show the main window in normal mode
            ui->widget_2->show(); // Show a specific widget
            if (this->width() < this->height()) {
                ui->widget_4->show(); // Show another widget if certain conditions are met
            }
            this->resize(this->width(), this->height() + 1); // Resize the window
            this->resize(this->width(), this->height() - 1); // Resize the window
            btn_screen_full->setIcon(QIcon(":/img/fullscreen.png")); // Set button icon to enter full-screen
            isfull = 0; // Update the full-screen state flag
        }
    });

}

void videowindow::initwindow()
{
    initializePlaylists();
    initializeMediaPlayer();
    initializeWidget();
}

// Initialize media playlists for video and list items in the videowindow.

void videowindow::initializePlaylists()
{
    // Create media playlists for video and list items
    list_bar = new QMediaPlaylist();
    video_bar = new QMediaPlaylist();

    // Set the playback mode to loop for both playlists
    video_bar->setPlaybackMode(QMediaPlaylist::Loop);
    list_bar->setPlaybackMode(QMediaPlaylist::Loop);

    // Connect a slot to handle the 'currentIndexChanged' signal of 'video_bar'.
    // This connection ensures that widget visibility is updated when the current media item changes.
    connect(video_bar, &QMediaPlaylist::currentIndexChanged, this, [=](int index) {
        updateWidgetVisibility();
    });
}
void videowindow::initializeMediaPlayer()
{
    play_area = new QMediaPlayer();
    play_area->setVolume(20);
    play_area->setPlaylist(list_bar);
}

// Initialize the video widget and set its visibility in the videowindow.

void videowindow::initializeWidget()
{
    // Create a video widget and set it as the video output for the media player
    widget_item = new QVideoWidget(ui->widget);
    play_area->setVideoOutput(widget_item);

    // Resize the video widget to match the size of the parent widget 'ui->widget'
    widget_item->resize(ui->widget->size());

    // Initialize the visibility of interface elements
    updateWidgetVisibility();
}

void videowindow::updateWidgetVisibility()
{
    QMediaContent currentMedia = video_bar->currentMedia();
    QString currentPlayerName = currentMedia.canonicalUrl().fileName();
    if (currentPlayerName.contains("mp3"))
    {
        ui->widget->hide();
    }
    else
    {
        ui->widget->show();
    }
}


void videowindow::createRecordList()
{
    record_list = new QWidget(this);
    record_list->resize(this->width() * 0.33, this->height());
    record_list->setMaximumWidth(400);
    record_list->setStyleSheet("background-color: rgb(41, 41, 43);");
    record_list->hide();

    Animation_setting = new QPropertyAnimation(record_list, "geometry");
    Animation_setting->setEasingCurve(QEasingCurve::InOutSine);
    Animation_setting->setDuration(150);

    list_name = new QLabel("Play History");
    list_name->setStyleSheet("color: rgb(255, 255, 255); font: 700 20pt 'Source Han Sans', 'Noto Sans CJK', '微软雅黑', Arial");

    item_list = new video_list();
    connect(item_list, &QTableWidget::cellDoubleClicked, this, &videowindow::connectCellDoubleClick);

    QVBoxLayout *movelayout = new QVBoxLayout(record_list);
    movelayout->addWidget(list_name);
    movelayout->addWidget(item_list);
    movelayout->setStretch(0, 1);
    movelayout->setStretch(1, 8);
}

// Connect to a cell's double-click event in the video list.

void videowindow::connectCellDoubleClick(int i, int j)
{
    // Get the widget in the specified cell (row 'i', column 'j')
    QWidget *widget = item_list->cellWidget(i, j);

    if (widget) {
        // Try to cast the widget to the 'list_widget' type
        list_widget *item = qobject_cast<list_widget *>(widget);

        if (item) {
            // Get the text from the label associated with the widget
            QString text = item->txt_catch();

            // Process the label text, for example, configure a URL
            this->url_config(text);
        }
    } else {
        // No stored widget found
    }
}


// Initialize the video list and connect a slot to handle cell double-click events.

void videowindow::list_init()
{
    // Create a new video list
    list = new video_list();

    // Connect a slot to handle the 'cellDoubleClicked' signal of the video list
    connect(list, &QTableWidget::cellDoubleClicked, this, [=](int i, int j) {
        // Get the widget in the specified cell (row 'i', column 'j')
        QWidget *widget = list->cellWidget(i, j);

        if (widget) {
            // Try to cast the widget to the 'list_widget' type
            list_widget *item = qobject_cast<list_widget *>(widget);

            if (item) {
                // Get the text from the label associated with the widget
                QString text = item->txt_catch();

                // Process the label text, for example, configure a URL
                this->url_config(text);
            }
        } else {
            // No stored widget found
        }
    });

    // Create a QVBoxLayout and add the video list to the 'ui->widget_4'
    QVBoxLayout *layout = new QVBoxLayout(ui->widget_4);
    layout->addWidget(list);
}

void videowindow::btn_config(QPushButton *btn, QSize size, QString iconpath)
{
    btn->setFlat(true);
    btn->resize(size);
    btn->setIcon(QIcon(iconpath));
    btn->setIconSize(btn->size());
}

// Configure the URL and media playback for the videowindow.

void videowindow::url_config(QString path)
{
    // Iterate through the video playlist to find the media file
    bool hasFile = false;
    int current = 0;
    for (int i = 0; i < video_bar->mediaCount(); ++i) {
        QMediaContent media = video_bar->media(i);
        QUrl url = media.canonicalUrl();
        QString fileName = url.fileName();
        if (fileName == QUrl(path).fileName()) {
            hasFile = true;
            current = i;
            break;
        }
    }

    if (hasFile) {
        // Set the current index to the found media file
        video_bar->setCurrentIndex(current);
    } else {
        // Add the media file to the playlist and set it as the current item
        video_bar->addMedia(QUrl(path));
        video_bar->setCurrentIndex(video_bar->mediaCount() - 1);

        // Set the status bar with the media file path
        bordervideopath->setText(QString("%1").arg(path));

        // Add the data associated with the media file
        add_data(path);
    }

    // Iterate through the list playlist to find the media file
    for (int i = 0; i < list_bar->mediaCount(); ++i) {
        QMediaContent media = list_bar->media(i);
        QUrl url = media.canonicalUrl();
        QString fileName = url.fileName();
        if (fileName == QUrl(path).fileName()) {
            hasFile = true;
            current = i;
            break;
        }
    }

    // Set the current index in the list playlist
    list_bar->setCurrentIndex(current);

    // Start playback
    play_area->play();

    // Enable the progress bar when playing
    if (slider_video->isEnabled() == false)
        slider_video->setEnabled(true);

    // Set the progress bar for continuous updating
    updataslider->start(200);
}



void videowindow::get_data(QString path)
{
    int currentIndex = findMediaIndex(path);

    if (currentIndex != -1) {
        video_bar->setCurrentIndex(currentIndex);
    } else {
        addMediaAndSetCurrentIndex(path);
        updateStatusBar(path);
        add_data(path);
    }
}

int videowindow::findMediaIndex(const QString& path)
{
    int current = -1;
    for (int i = 0; i < video_bar->mediaCount(); ++i) {
        QMediaContent media = video_bar->media(i);
        QUrl url = media.canonicalUrl();
        QString fileName = url.fileName();
        if (fileName == QUrl(path).fileName()) {
            current = i;
            break;
        }
    }
    return current;
}

void videowindow::addMediaAndSetCurrentIndex(const QString& path)
{
    video_bar->addMedia(QUrl(path));
    video_bar->setCurrentIndex(video_bar->mediaCount() - 1);
}

void videowindow::updateStatusBar(const QString& path)
{
    bordervideopath->setText(QString("%1").arg(path));
}


// Add data associated with a media file to the videowindow.

void videowindow::add_data(QString name)
{
    // Get the current row count in the item_list
    int row = item_list->rowCount();

    // Set the column count of the item_list before adding data
    item_list->setColumnCount(1);

    // Insert a new row into the item_list
    item_list->insertRow(row);

    // Create a list_widget item with the given name and row
    list_widget* item = createListWidget(name, row);

    // Set the created list_widget item as a cell widget in the item_list
    item_list->setCellWidget(row, 0, item);

    // Connect signals and slots for the list_widget item
    connectItemSignals(item);
}

list_widget* videowindow::createListWidget(const QString& name, int row)
{
    list_widget* item = new list_widget();
    item->name_setting("moveitem_listitem");
    item->txt_setting(name);
    item->favorite_setting(QSize(20, 20));
    item->resize(item_list->width(), item_list->columnWidth(row));
    item->icon_setting(QIcon(":/img/aixin.png"));
    item->is_like("unlike");
    return item;
}

void videowindow::connectItemSignals(list_widget* item)
{
    connect(item, &list_widget::video_play, this, [=]() {
        this->url_config(item->txt_catch());
    });
}



// Get the list of media files in the specified directory and store their URLs.

void videowindow::get_url(const QString &directory)
{
    // Create a QDir object for the specified directory
    QDir dir(directory);

    // Create a QStringList to store file filters based on the operating system
    QStringList filters;

    // Use preprocessor directives to distinguish different operating systems
#if defined(_WIN32)
    filters << "*.wmv";
#else
    filters << "*.mp4" << "*.mov";
#endif

    // Get the list of files in the directory that match the specified filters
    QStringList files = dir.entryList(filters, QDir::Files);

    // Iterate through the list of files
    foreach (const QString &file, files) {
        // Create a file URL from the local file path
        QUrl fileUrl = QUrl::fromLocalFile(dir.filePath(file));

        // Debug print the file path
        qDebug() << dir.filePath(file);

        // Store the file URL in the 'array_url' QVector
        array_url.push_back(fileUrl.toString());
    }
}

// Set up data and populate the provided QTableWidget with media items.

void videowindow::data_setting(QTableWidget* list)
{
    // Clear the existing rows and set the column count
    list->setRowCount(0);
    list->setColumnCount(1);

    // Iterate through the 'array_url' QVector and add media items to the list
    for (int i = 0; i < array_url.size(); i++)
    {
        // Insert a new row in the list
        list->insertRow(i);

        // Set the row height for better display
        list->setRowHeight(i, 100);

        // Create a list_widget item with the media URL and row index
        list_widget* item = createListWidget(array_url.at(i), i);

        // Set the list_widget item as a cell widget in the list
        list->setCellWidget(i, 0, item);

        // Add the media to the playback list
        list_bar->addMedia(QUrl(array_url.at(i)));

        // Connect signals and slots for the list_widget item
        connectItemSignals(item);
    }
}



void videowindow::button_responsive()
{
    if(ui->widget_4->isVisible())
    {
        btn_file_op->setIconSize(QSize(this->width()*0.05,this->width()*0.05));
        btn_back->setIconSize(QSize(this->width()*0.05,this->width()*0.05));
        btn_forward->setIconSize(QSize(this->width()*0.05,this->width()*0.05));
        btn_record->setIconSize(QSize(this->width()*0.05,this->width()*0.05));
        space->setIconSize(QSize(this->width()*0.05,this->width()*0.05));
        btn_volume->setIconSize(QSize(this->width()*0.05,this->width()*0.05));
        btn_play->setIconSize(QSize(this->width()*0.052,this->width()*0.052));
        btn_next->setIconSize(QSize(this->width()*0.06,this->width()*0.06));
        btn_last->setIconSize(QSize(this->width()*0.06,this->width()*0.06));
        btn_screen_full->setIconSize(QSize(this->width()*0.04,this->width()*0.04));
    }
}

// Resize event handler for adjusting the window and its components.

void videowindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    // Resize the video playback area and control center
    widget_item->resize(ui->widget->size());
    btn_list->resize(ui->widget_2->size());

    // Resize and hide the playback queue
    record_list->resize(this->width() * 0.3, this->height());
    record_list->hide();

    // Show/hide the control panel based on window width and height
    if (this->width() > this->height()) {
        ui->widget_4->hide();
    } else {
        ui->widget_4->show();
    }

    // Adjust button icon sizes
    button_responsive();
}


// Mouse double-click event handler for toggling fullscreen mode.

void videowindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (ui->widget_2->isVisible())
        {
            // Enter fullscreen mode
            toggleFullScreen(true);
        }
        else
        {
            // Exit fullscreen mode
            toggleFullScreen(false);
        }
    }

    // Call the base class implementation to maintain other default behaviors
    QWidget::mouseDoubleClickEvent(event);
}

void videowindow::toggleFullScreen(bool fullScreen)
{
    if (fullScreen)
    {
        ui->widget_2->hide();
        ui->widget_4->hide();
        this->showFullScreen();
    }
    else
    {
        this->showNormal();
        ui->widget_2->show();
        if (this->width() < this->height())
            ui->widget_4->show();
        this->resize(this->width(), this->height() + 1);
        this->resize(this->width(), this->height() - 1);
    }
}


void videowindow::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
}

void videowindow::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
}

void videowindow::mousePressEvent(QMouseEvent *event)
{
    if(!ishide)
    {
        Animation_setting->setStartValue(QRect(this->rect().width()-record_list->width(),0,record_list->width(),record_list->height()));
        Animation_setting->setEndValue(QRect(this->rect().width(),0,record_list->width(),record_list->height()));
        Animation_setting->start();
        ishide = true;
    }
}

void videowindow::border_bar_init()
{
    initTimeLabel();
    initBorderVideoLabels();
}

void videowindow::initTimeLabel()
{
    QLabel *timelabel = new QLabel("", ui->statusbar);
    timelabel->setStyleSheet("font-family: Microsoft YaHei; font-size: 16px;");
    QTimer *timer = new QTimer();
    ui->statusbar->addPermanentWidget(timelabel, 0);
    connect(timer, &QTimer::timeout, this, [=]() {
        QDateTime currentDateTime = QDateTime::currentDateTime();
        QString currentDateTimeString = currentDateTime.toString("  hh:mm:ss   \nyyyy/MM/dd   ");
        timelabel->setText(currentDateTimeString);
    });
    timer->start(100);
}

void videowindow::initBorderVideoLabels()
{
    bordervideo = new QLabel("Playing：", ui->statusbar);
    ui->statusbar->addWidget(bordervideo, 0);

    bordervideopath = new QLabel("None", ui->statusbar);
    ui->statusbar->addWidget(bordervideopath, 0);
}





void videowindow::init_upper_border()
{
    upper_menu();
    upper_menu_connections();
}

void videowindow:: upper_menu()
{
    menu_bar = new QMenuBar(this);
    this->setMenuBar(menu_bar);
    language_list = new QMenu("Language",menu_bar);
    background_list = new QMenu("Background",menu_bar);
    speed_list_item = new QMenu("Speed",menu_bar);//Build menu bar options
    speed_list = new QMenu("...",menu_bar);//Build menu bar options

    chinese = new QAction("Chinese");
    english = new QAction("English");
    custom = new QAction("DIY");

    like = new QAction(QIcon(":/img/aixin.png"),"Like");
    picture = new QAction(QIcon(":/img/photo.png"),"Screen Photo");

    halfspeed = new QAction("0.5 speed");
    onespeed = new QAction("1 speed");
    twospeed = new QAction("2 speed");

    language_list->addAction(chinese);
    language_list->addSeparator();
    language_list->addAction(english);
    language_list->addSeparator();

    background_list->addAction(custom);

    speed_list->addSeparator();
    speed_list->addAction(like);//Adds a function to the menu bar elective
    speed_list->addSeparator();//separator
    speed_list->addAction(picture);//Adds a function to the menu bar elective
    speed_list->addSeparator();//separator

    speed_list_item->addAction(halfspeed);
    speed_list_item->addAction(onespeed);
    speed_list_item->addAction(twospeed);

    menu_bar->addMenu(language_list);//Adds menu bar options to the menu bar
    menu_bar->addMenu(background_list);
    menu_bar->addMenu(speed_list_item);
    menu_bar->addMenu(speed_list);

    menu_bar->show();

    menu_bar->setStyleSheet("QMenu {"
                            "color:black;"
                            "background-color:white;"
                            "border-radius:3px;"
                            "padding:5px;"
                            "margin:6px;"
                            "}"
                            "QMenu::item:text {"
                            "font-size:15px;"
                            "padding-left:10px;"
                            "padding-right:10px;"
                            "}"
                            "QMenu::item:selected {"
                            "color:#1aa3ff;"
                            "background-color: #e5f5ff;"
                            "border-radius:3px;"
                            "}"
                            "QMenu::separator {"
                            "height:1px;"
                            "background:#bbbbbb;"
                            "margin:5px;"
                            "margin-left:10px;"
                            "margin-right:10px;"
                            "}");


}

void videowindow:: upper_menu_connections()
{
    connect(custom,&QAction::triggered,this,[=](){
        QColor color = QColorDialog::getColor(Qt::white, this);
        if(!color.isValid()){
        }
        else {
            ui->centralwidget->setStyleSheet("background-color: "+ color.name()+ ";");
        }
    });
    connect(like,&QAction::triggered,this,[=](){
        if(whetherfavorite==0){
            qDebug()<<"ok";
            whetherfavorite = 1;
            like->setIcon(QIcon(":/img/aixin2.png"));
        }
        else{
            whetherfavorite = 0;
            like->setIcon(QIcon(":/img/aixin.png"));
        }
    });
    connect(picture,&QAction::triggered,this,[=](){
        QString filePath = QFileDialog::getSaveFileName(
            this,
            tr("Save screenshot"),
            "",
            tr("JPEG Files (*.jpg);;PNG Files (*.png)"));
        if (!filePath.isEmpty()) {
            QPixmap screenshot = ui->centralwidget->grab();  // screenshot
            screenshot.save(filePath, Q_NULLPTR, 100);  // Save the screenshot and set the quality parameter to 100
        }
    });
    connect(halfspeed,&QAction::triggered,this,[=](){
        play_area->setPlaybackRate(0.5);
    });
    connect(onespeed,&QAction::triggered,this,[=](){
        play_area->setPlaybackRate(1.0);
    });
    connect(twospeed,&QAction::triggered,this,[=](){
        play_area->setPlaybackRate(2.0);
    });
    connect(english,&QAction::triggered,this,[=](){
        language_list->setTitle("Language");
        background_list->setTitle("Background");
        speed_list_item->setTitle("Speed");
        chinese->setText("Chinese");
        english->setText("English");
        custom->setText("DIY");
        like->setText("Like");
        picture->setText("Screen Photo");
        halfspeed->setText("0.5 speed");
        onespeed->setText("1.0 speed");
        twospeed->setText("2.0 speed");
        bordervideo->setText("loacal Playing：");
        list->setTitle("Play List");
        item_list->setTitle("Play List");
        list_name->setText("Play History");
    });
    connect(chinese,&QAction::triggered,this,[=](){
        language_list->setTitle("语言");
        background_list->setTitle("背景");
        speed_list_item->setTitle("倍速");
        chinese->setText("中文");
        english->setText("英文");
        custom->setText("自定义");
        like->setText("收藏");
        picture->setText("截图");
        halfspeed->setText("0.5倍速");
        onespeed->setText("1倍速");
        twospeed->setText("2倍速");
        bordervideo->setText("播放：");
        list->setTitle("播放列表");
        item_list->setTitle("播放历史");
        list_name->setText("播放历史");
    });
}

void videowindow::Volumeadjust(QString path)
{
    QSoundEffect *soundEffect = new QSoundEffect();
    soundEffect->setSource(QUrl::fromLocalFile(path));
    soundEffect->setVolume(1.0);
    soundEffect->play();
}

