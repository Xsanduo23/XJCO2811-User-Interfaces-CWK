#include "videowindow.h"
#include "ui_videowindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QTimer>
#include <qDebug>
#include <QToolButton>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QColorDialog>
#include <QDateTime>
#include <QMessageBox>
#include <QShortcut>
int i =0;

// Constructor for the 'videowindow' class.
// This constructor initializes an instance of the 'videowindow' class with the given 'parent' widget.
// It sets up the user interface, initializes playback devices and widgets, and configures timers and initial settings.

videowindow::videowindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::videowindow)
{
    ui->setupUi(this);

    // Set the window icon
    this->setWindowIcon(QIcon(":/img/logo.png"));

    // Set the initial window position
    this->move(380, 2);

    updataslider = new QTimer();

    // Initialize playback devices and widgets
    initwindow();

    // Initialize the user interface
    init_widget();

    // Create a timer to update the playback progress and time display



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
}

videowindow::~videowindow()
{
    delete ui;
}

void videowindow::init_widget()
{
    // Set the initial volume level
    value_volume = 20;

    // Create a container for buttons
    btn_list = new QWidget(ui->menu_container);
    btn_list->resize(ui->menu_container->size());
    player1 = new QMediaPlayer(this);
    player2 = new QMediaPlayer(this);
    // Set the fixed height for 'menu_container'
    ui->menu_container->setFixedHeight(120);

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
        {&btn_screen_full, 0.05, ":/img/fullscreen.png"},
        {&btn_file_op, 0.05, ":/img/wenjianjia.png"},
        {&btn_back, 0.05, ":/img/backward-full.png"},
        {&btn_forward, 0.05, ":/img/go-full.png"},
        {&btn_play, 0.05, ":/img/zanting.png"},
        {&FAQ, 0.05, ":/img/FAQ.png"},
        {&btn_last, 0.05, ":/img/prev-full.png"},
        {&btn_next, 0.05, ":/img/next-full.png"},
        {&btn_volume, 0.05, ":/img/24gf-volumeHigh.png"},
        {&btn_record, 0.05, ":/img/24gl-playlistVideo.png"}
    };

    // Initialize buttons using a loop
    for (auto& button : buttons) {
        *button.btn = new QPushButton();
        (*button.btn)->setFlat(true);
        (*button.btn)->resize(QSize(this->width() * button.sizeFactor + 10, this->width() * button.sizeFactor + 10));
        (*button.btn)->setIcon(QIcon(button.imagePath));
        (*button.btn)->setIconSize(QSize(this->width() * button.sizeFactor + 10, this->width() * button.sizeFactor + 10));
        (*button.btn)->setObjectName("yes");
    }

    // Additional configurations
    FAQ->setStyleSheet("");

    slider_volume = new QSlider(this);
    slider_volume->setValue(30);
    slider_volume->setPageStep(1);
    slider_volume->resize(10,10);
    slider_volume->setOrientation(Qt::Horizontal);
    slider_volume->setMinimumWidth(30);
    slider_volume->setMaximumWidth(100);

    // Assume that btn_file_op, FAQ, btn_back,... btn_record has been created
    QWidget *leftWidgets[] = { btn_file_op, FAQ };
    QWidget *centerWidgets[] = { btn_back, btn_last, btn_play, btn_next, btn_forward };
    QWidget *rightWidgets[] = { btn_screen_full, btn_volume, slider_volume, btn_record };

    QHBoxLayout *first_layout = new QHBoxLayout();

    // Add left component
    for (QWidget *widget : leftWidgets) {
        first_layout->addWidget(widget);
    }

    // Adding the first extended blank item pushes the button to the center
    first_layout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    // Add the middle button
    for (QWidget *widget : centerWidgets) {
        first_layout->addWidget(widget);
    }

    // Add a second extension blank item
    first_layout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    // Add the right component
    for (QWidget *widget : rightWidgets) {
        first_layout->addWidget(widget);
    }

    first_layout->setSpacing(0);



    slider_video = new QSlider();
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
    slider_video->setMaximum(300);
    slider_video->setPageStep(1);
    slider_video->setOrientation(Qt::Horizontal);

    QHBoxLayout *layout_t = new QHBoxLayout();
    layout_t->addWidget(time);
    layout_t->addWidget(slider_video);

    QVBoxLayout *layout_list = new QVBoxLayout(btn_list);

    layout_list->addLayout(layout_t);
    layout_list->addLayout(first_layout);

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


    // Disable dragging of the initial progress bar
    slider_video->setEnabled(false);
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
                slider_video->setValue(int(play_area->position() * 300 / play_area->duration()));
            }
        } catch (const std::exception& e) {
            // Handle exceptions, e.g., output error messages or perform error handling
        }
    });

    // Connect a slot to the 'clicked' signal of the 'btn_play' button.
    // This connection allows toggling the playback state when the button is clicked.
    // If the media player is not in the playing state, it starts playing and updates the button icon.
    // If the media player is already playing, it pauses playback and updates the button icon.

    connect(btn_play, &QPushButton::clicked, this, [=]() {
        QSize currentIconSize = QSize(btn_play->iconSize());
        if (play_area->state() != QMediaPlayer::PlayingState) {
            // Start playback, update the button icon to 'pause', and log a message
            play_area->play();
            qDebug() << "start";
            btn_play->resize(currentIconSize);
            btn_play->setIcon(QIcon(":/img/zanting.png"));
            btn_play->setIconSize(currentIconSize);
        } else {
            play_area->pause();
            qDebug() << "stop";
            // Pause playback, update the button icon to 'play', and log a message
            btn_play->resize(currentIconSize);
            btn_play->setIcon(QIcon(":/img/bofang.png"));
            btn_play->setIconSize(currentIconSize);

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
        play_area->setPosition(slider_video->value() * play_area->duration() / 300);
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
            btn_volume->resize(QSize(btn_volume->iconSize()));
            btn_volume->setIcon(QIcon(":/img/24gf-volumeHigh.png"));
            play_area->setVolume(value_volume); // Set volume value
            slider_volume->setValue(value_volume); // Set slider position
            qDebug() << "Volume enabled, value: " << value_volume;
        } else {
            // Toggle the volume button state to "no" and set the slider value to 0, while updating the button icon
            btn_volume->setObjectName("no"); // Disable volume            
            btn_volume->resize(QSize(btn_volume->iconSize()));
            btn_volume->setIcon(QIcon(":/img/24gf-volumeDisable.png"));
            slider_volume->setValue(0); // Set slider position to mute
            qDebug() << "Volume disabled";
        }
    });



    // Connect a slot to the 'valueChanged' signal of 'slider_volume'.
    // This connection handles changes in the volume slider's value.

    connect(slider_volume, &QSlider::valueChanged, this, [=](int value) {
        // Set the media player's volume based on the slider value
        play_area->setVolume(value);
        if (value == 0) {
            btn_volume->setIcon(QIcon(":/img/24gf-volumeDisable.png"));
        } else {
            btn_volume->setIcon(QIcon(":/img/24gf-volumeHigh.png"));
        }

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
        QDialog *instructionDialog = new QDialog(this);
        QVBoxLayout *vLayout = new QVBoxLayout(instructionDialog);
        QLabel *label = new QLabel("CineSync Leeds是一款基于qt5.15.2的视频播放器，\n"
                                   "支持Windows/mac os, linux播放wmv/mp4, MOV格式的视频。\n"
                                   "视频的界面由播放区、菜单和播放列表组成。视频功能包括暂停播放、\n"
                                   "快进快退、视频上下切换、声音调节、\n"
                                   "全屏切换、双速播放、点赞、截图、自定义界面背景颜色、\n"
                                   "切换语言。同时，程序界面满足自适应需求，适合任何屏幕尺寸。\n"
                                   "\n\n系统要求:\n"
                                   "1. Windows 10/11, macOS, Linux\n"
                                   "2. 至少4GB内存\n"
                                   "-----------------------------------------------------------------------\n"
                                   "CineSync Leeds is a video player based on qt5.15.2, \n"
                                   "which supports Windows/mac os, linux to play wmv/mp4, MOV format video. \n"
                                   "The interface of a video consists of a playarea, a menu and a playlist. \n"
                                   "Video functions include pause play, fast forward and backward, \n"
                                   "switch up and down video, sound adjustment, \n"
                                   "switch full screen, double speed playback, like, screenshot, \n"
                                   "customize the interface background color, \n"
                                   "switch language. At the same time, the program interface meets \n"
                                   "the needs of adaptive, suitable for any screen size.\n"
                                   "\n\nSystem Requirement:\n"
                                   "1. Windows 10/11, macOS, Linux\n"
                                   "2. At least 4GB of RAM");
        vLayout->addWidget(label);


        QHBoxLayout *hLayout = new QHBoxLayout();
        QPushButton *voiceButton1 = new QPushButton("Chinese");
        QPushButton *voiceButton2 = new QPushButton("English");
        QPushButton *stopButton = new QPushButton("Stop");


        hLayout->addWidget(voiceButton1);
        hLayout->addWidget(voiceButton2);
        hLayout->addWidget(stopButton);


        vLayout->addLayout(hLayout);


        connect(voiceButton1, &QPushButton::clicked, this, [=]() {
            player1->stop();
            player1->setMedia(QUrl("qrc:/music/Chinese.wav"));
            player1->setPosition(0);
            connect(player1, &QMediaPlayer::mediaStatusChanged, this, [=](QMediaPlayer::MediaStatus status) {
                if (status == QMediaPlayer::BufferedMedia || status == QMediaPlayer::LoadedMedia) {
                    player1->play();
                }
            });
        });

        connect(voiceButton2, &QPushButton::clicked, this, [=]() {
            player2->stop();
            player2->setMedia(QUrl("qrc:/music/English.wav"));
            player2->setPosition(0);
            connect(player2, &QMediaPlayer::mediaStatusChanged, this, [=](QMediaPlayer::MediaStatus status) {
                if (status == QMediaPlayer::BufferedMedia || status == QMediaPlayer::LoadedMedia) {
                    player2->play();
                }
            });
        });

        connect(stopButton, &QPushButton::clicked, this, [=]() {
            player1->stop();
            player2->stop();
        });
        instructionDialog->setLayout(vLayout);
        instructionDialog->exec();
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


    connect(btn_forward, &QPushButton::clicked, this, [=]() {
        if (play_area->state() == QMediaPlayer::PlayingState) {
            // Check if the media player is in the playing state

            // Calculate the new position for fast-forwarding
            qint64 newPosition = play_area->position() + 2000;

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
            qint64 newPosition = play_area->position() - 2000;

            // Set the new playback position
            play_area->setPosition(newPosition);

            // Additional code, such as displaying a message
            qDebug() << "Back button clicked, current playback position: " << newPosition;
        }
    });


    QShortcut *play_pause = new QShortcut(QKeySequence(Qt::Key_Space), btn_play);
    connect(play_pause, &QShortcut::activated, this, [=]() {
        if (play_area->state() == QMediaPlayer::PlayingState) {
            play_area->pause();
            btn_play->setIcon(QIcon(":/img/bofang.png"));
        } else {
            play_area->play();
            btn_play->setIcon(QIcon(":/img/zanting.png"));
        }
    });


    // shortcut for volume down
    QShortcut *volumeDownShortcut = new QShortcut(QKeySequence(Qt::Key_Down), this);
    connect(volumeDownShortcut, &QShortcut::activated, this, [=](){
        play_area->setVolume(play_area->volume() - 10);
        slider_volume->setValue(play_area->volume());
    });
    // shortcut for volume up
    QShortcut *volumeUpShortcut = new QShortcut(QKeySequence(Qt::Key_Up), this);
    connect(volumeUpShortcut, &QShortcut::activated, this, [=](){
        play_area->setVolume(play_area->volume() + 10);
        slider_volume->setValue(play_area->volume());
    });


    // Shortcut: Left Arrow Key (Video Backward 2 Seconds)
    QShortcut *backwardShortcut = new QShortcut(QKeySequence(Qt::Key_Left), this);
    connect(backwardShortcut, &QShortcut::activated, this, [=]() {
        // Handle logic for video backward 2 seconds here
        qint64 newPosition = play_area->position() - 2000; // 2000 milliseconds = 2 seconds
        if (newPosition < 0) {
            newPosition = 0; // Ensure not to rewind to a negative position
        }
        play_area->setPosition(newPosition);
    });

    // Shortcut: Right Arrow Key (Video Forward 2 Seconds)
    QShortcut *forwardShortcut = new QShortcut(QKeySequence(Qt::Key_Right), this);
    connect(forwardShortcut, &QShortcut::activated, this, [=]() {
        // Handle logic for video forward 2 seconds here
        qint64 newPosition = play_area->position() + 2000; // 2000 milliseconds = 2 seconds
        qint64 duration = play_area->duration();
        if (newPosition > duration) {
            newPosition = duration; // Ensure not to exceed the total video duration
        }
        play_area->setPosition(newPosition);
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
    play_area->setPlaylist(list_bar);
    play_area->setVolume(30);
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
    record_list->setMaximumWidth(500);
    record_list->setStyleSheet("background-color: rgb(41, 41, 43);");
    record_list->hide();

    Animation_setting = new QPropertyAnimation(record_list, "geometry");
    Animation_setting->setEasingCurve(QEasingCurve::InOutSine);
    Animation_setting->setDuration(150);

    QVBoxLayout *layout = new QVBoxLayout(record_list);
    list_name = new QLabel("Play History");
    list_name->setStyleSheet("color: rgb(255, 255, 255); "
                             "font: 700 20pt 'Source Han Sans', 'Noto Sans CJK', '微软雅黑', Arial");
    layout->addWidget(list_name);
    item_list = new video_list();
    connect(item_list, &QTableWidget::cellDoubleClicked, this, &videowindow::connectCellDoubleClick);
    layout->addWidget(item_list);
    layout->setStretch(0, 1);
    layout->setStretch(1, 7);
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

// Configure the URL and media playback for the videowindow.
void videowindow::url_config(QString path)
{
    // Function to find the index of a media file in a playlist
    auto findMediaIndex = [](QMediaPlaylist* playlist, const QString& path) -> int {
        for (int i = 0; i < playlist->mediaCount(); ++i) {
            QMediaContent media = playlist->media(i);
            QUrl url = media.canonicalUrl();
            if (url.fileName() == QUrl(path).fileName()) {
                return i;
            }
        }
        return -1;
    };

    // Find the index of the media file in video_bar playlist
    int videoIndex = findMediaIndex(video_bar, path);

    if (videoIndex == -1) {
        // Media file not found in video_bar playlist, add it
        video_bar->addMedia(QUrl(path));
        videoIndex = video_bar->mediaCount() - 1;

        // Set status bar and add data
        QFileInfo fileInfo(path);
        QString filename = fileInfo.fileName();
        bordervideopath->setText(filename);
        add_data(path);
    }

    // Set the current index in video_bar playlist
    video_bar->setCurrentIndex(videoIndex);

    // Find the index of the media file in list_bar playlist
    int listIndex = findMediaIndex(list_bar, path);

    // Set the current index in list_bar playlist
    list_bar->setCurrentIndex(listIndex);

    // Start playback and enable progress bar
    play_area->play();
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
    list_widget* this_list = new list_widget();
    this_list->txt_setting(name);
    this_list->favorite_setting(QSize(20, 20));
    this_list->resize(item_list->width(), item_list->columnWidth(row));
    this_list->icon_setting(QIcon(":/img/aixin.png"));
    this_list->is_like("unlike");
    return this_list;
}

void videowindow::connectItemSignals(list_widget* item)
{
    connect(item, &list_widget::video_play, this, [=]() {
        this->url_config(item->txt_catch());
    });
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
        list_widget* this_list = createListWidget(array_url.at(i), i);

        // Set the list_widget item as a cell widget in the list
        list->setCellWidget(i, 0, this_list);

        // Add the media to the playback list
        list_bar->addMedia(QUrl(array_url.at(i)));

        // Connect signals and slots for the list_widget item
        connectItemSignals(this_list);
    }
}

// Resize event handler for adjusting the window and its components.
void videowindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    // Show/hide the control panel based on window width and height
    if (this->width() > this->height()) {
        ui->list_container->hide();
    } else {
        ui->list_container->show();
    }

    if(ui->list_container->isVisible())
    {
        QList<QPushButton*> buttons;
        buttons.append(btn_file_op);
        buttons.append(btn_back);
        buttons.append(btn_forward);
        buttons.append(btn_record);
        buttons.append(FAQ);
        buttons.append(btn_volume);
        buttons.append(btn_play);
        buttons.append(btn_next);
        buttons.append(btn_last);
        buttons.append(btn_screen_full);

        const double iconSizeFactor = 0.05; // 调整图标大小的系数

        for (QPushButton* button : buttons) {
            QSize iconSize(this->width() * iconSizeFactor, this->width() * iconSizeFactor);
            button->setIconSize(iconSize);
        }
    }

    // Resize the video playback area and control center
    widget_item->resize(ui->play_container->size());
    btn_list->resize(ui->menu_container->size());

    // Resize and hide the playback queue
    record_list->resize(this->width() * 0.33, this->height());
    record_list->hide();
}

void videowindow::border_bar_init()
{
    initBorderVideoLabels();
}

void videowindow::initBorderVideoLabels()
{
    ui->statusbar->setStyleSheet("background-color: #60669B;"
                                 " color: #ffffff;"
                                 " font-family: Arial;"
                                 " height: 12px;"
                                 " font-weight: 500;");
    bordervideo = new QLabel("Now：", ui->statusbar);
    bordervideo->setStyleSheet("color:white;");
    ui->statusbar->addWidget(bordervideo, 0);

    bordervideopath = new QLabel("None", ui->statusbar);
    ui->statusbar->addWidget(bordervideopath, 0);
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
}

void videowindow::mousePressEvent(QMouseEvent *event)
{
    if (ishide == false)
    {
        QRect startRect(this->rect().width() - record_list->width(), 0, record_list->width(), record_list->height());
        QRect endRect(this->rect().width(), 0, record_list->width(), record_list->height());

        Animation_setting->setStartValue(startRect);
        Animation_setting->setEndValue(endRect);

        Animation_setting->setDuration(500);

        Animation_setting->start();

        ishide = true;
    }
}


