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
#include <QMessageBox>
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

    // Set the fixed height for 'menu_container'
    ui->menu_container->setFixedHeight(120);

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
            Volumeadjust(":/music/start.wav");
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
    // Create a container for buttons
    btn_list = new QWidget(ui->menu_container);
    btn_list->resize(ui->menu_container->size());

    // Initialize a label for time display
    time = new QLabel("00:00/00:00");
    time->setStyleSheet("color:white");

    // Array of button information
    struct ButtonInfo {
        QPushButton** btn;
        double sizeFactor;
        const char* imagePath;
    };

    ButtonInfo buttons[] = {
        {&btn_screen_full, 0.04, ":/img/fullscreen.png"},
        {&btn_file_op, 0.05, ":/img/wenjianjia.png"},
        {&btn_back, 0.04, ":/img/backward-full.png"},
        {&btn_forward, 0.04, ":/img/go-full.png"},
        {&btn_play, 0.039, ":/img/zanting.png"},
        {&FAQ, 0.05, ":/img/FAQ.png"},
        {&btn_last, 0.06, ":/img/prev-full.png"},
        {&btn_next, 0.06, ":/img/next-full.png"},
        {&btn_volume, 0.05, ":/img/24gf-volumeHigh.png"},
        {&btn_record, 0.05, ":/img/24gl-playlistVideo.png"}
    };

    // Initialize buttons using a loop
    for (auto& button : buttons) {
        *button.btn = new QPushButton();
        btn_config(*button.btn, QSize(this->width() * button.sizeFactor, this->width() * button.sizeFactor), button.imagePath);
        (*button.btn)->setObjectName("yes");
    }

    // Additional configurations
    FAQ->setStyleSheet("");

    slider_volume = new QSlider(this);
    slider_volume->resize(15,10);
    slider_volume->setMaximum(100);
    slider_volume->setPageStep(10);
    slider_volume->setOrientation(Qt::Horizontal);
    slider_volume->setValue(20);
    slider_volume->setMinimumWidth(70);
    slider_volume->setMaximumWidth(100);

    // 假设 btn_file_op, FAQ, btn_back, ... btn_record 已经被创建
    QWidget *leftWidgets[] = { btn_file_op, FAQ };
    QWidget *centerWidgets[] = { btn_back, btn_last, btn_play, btn_next, btn_forward };
    QWidget *rightWidgets[] = { btn_screen_full, btn_volume, slider_volume, btn_record };

    QHBoxLayout *hlayout = new QHBoxLayout();

    // 添加左侧部件
    for (QWidget *widget : leftWidgets) {
        hlayout->addWidget(widget);
    }

    // 添加第一个扩展空白项将按钮推向中间
    hlayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    // 添加中间的按钮
    for (QWidget *widget : centerWidgets) {
        hlayout->addWidget(widget);
    }

    // 添加第二个扩展空白项
    hlayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    // 添加右侧部件
    for (QWidget *widget : rightWidgets) {
        hlayout->addWidget(widget);
    }

    hlayout->setSpacing(0);



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

    QVBoxLayout *vlayout = new QVBoxLayout(btn_list);

    vlayout->addLayout(hlayout1);
    vlayout->addLayout(hlayout);

    btn_list->resize(ui->menu_container->size());
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
        if (!ui->list_container->isVisible()) {
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


    // Connect a slot to the 'clicked' signal of the 'FAQ' button.
    // This connection handles the functionality of toggling the playback mode between playlist loop and current item in loop.

    connect(FAQ, &QPushButton::clicked, this, [=]() {
        // 创建一个对话框
        QMessageBox *instructionBox = new QMessageBox(this);

        // 设置对话框的样式表，包括背景颜色和文本颜色
        instructionBox->setStyleSheet("QMessageBox { background-color: black; color: white; }"
                                      "QLabel { color: white; }"
                                      "QPushButton { color: black; }");

        // 设置对话框的大小
        instructionBox->setFixedSize(400, 200);

        // 设置对话框的字体
        QFont font;
        font.setFamily("Arial");
        font.setPointSize(12);
        instructionBox->setFont(font);

        // 设置对话框标题
        instructionBox->setWindowTitle("Instructions");

        // 设置对话框显示的文本
        instructionBox->setText("CineSync Leeds is a video player based on qt5.15.2, "
                                "which supports Windows/mac os, linux to play wmv/mp4, MOV format video. "
                                "The interface of a video consists of a playarea, a menu and a playlist. "
                                "Video functions include pause play, fast forward and backward, switch up and down video, sound adjustment, "
                                "switch full screen, double speed playback, like, screenshot, customize the interface background color, "
                                "switch language. At the same time, the program interface meets the needs of adaptive, suitable for any screen size."
                                "\n\nSystem Requirement:\n"  // 小标题
                                "1. Windows 10/11, macOS, Linux\n"  // 系统要求列表
                                "2. At least 4GB of RAM");  // 更多要求

        // 设置对话框的标准按钮为自定义文本
        instructionBox->setStandardButtons(QMessageBox::Ok);
        instructionBox->button(QMessageBox::Ok)->setText("Confirm");

        // 显示对话框
        instructionBox->show();
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
            ui->list_container->hide(); // Hide a specific widget
            this->showFullScreen(); // Show the main window in full-screen mode
            btn_screen_full->setIcon(QIcon(":/img/exitfullscreen.png")); // Set button icon to exit full-screen
            isfull = 1; // Update the full-screen state flag
        } else {
            // Exit full-screen mode
            this->showNormal(); // Show the main window in normal mode
            ui->menu_container->show(); // Show a specific widget
            if (this->width() < this->height()) {
                ui->list_container->show(); // Show another widget if certain conditions are met
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
    widget_item = new QVideoWidget(ui->play_container);
    play_area->setVideoOutput(widget_item);

    // Resize the video widget to match the size of the parent widget 'ui->widget'
    widget_item->resize(ui->play_container->size());

    // Initialize the visibility of interface elements
    updateWidgetVisibility();
}

void videowindow::updateWidgetVisibility()
{
    QMediaContent currentMedia = video_bar->currentMedia();
    QString currentPlayerName = currentMedia.canonicalUrl().fileName();
    if (currentPlayerName.contains("mp3"))
    {
        ui->play_container->hide();
    }
    else
    {
        ui->play_container->show();
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

    // Create a QVBoxLayout and add the video list to the 'ui->list_container'
    QVBoxLayout *layout = new QVBoxLayout(ui->list_container);
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
    if(ui->list_container->isVisible())
    {
        btn_file_op->setIconSize(QSize(this->width()*0.05,this->width()*0.05));
        btn_back->setIconSize(QSize(this->width()*0.05,this->width()*0.05));
        btn_forward->setIconSize(QSize(this->width()*0.05,this->width()*0.05));
        btn_record->setIconSize(QSize(this->width()*0.05,this->width()*0.05));
        FAQ->setIconSize(QSize(this->width()*0.05,this->width()*0.05));
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
    widget_item->resize(ui->play_container->size());
    btn_list->resize(ui->menu_container->size());

    // Resize and hide the playback queue
    record_list->resize(this->width() * 0.3, this->height());
    record_list->hide();

    // Show/hide the control panel based on window width and height
    if (this->width() > this->height()) {
        ui->list_container->hide();
    } else {
        ui->list_container->show();
    }

    // Adjust button icon sizes
    button_responsive();
}


// Mouse double-click event handler for toggling fullscreen mode.

void videowindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (ui->menu_container->isVisible())
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
        ui->menu_container->hide();
        ui->list_container->hide();
        this->showFullScreen();
    }
    else
    {
        this->showNormal();
        ui->menu_container->show();
        if (this->width() < this->height())
            ui->list_container->show();
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





