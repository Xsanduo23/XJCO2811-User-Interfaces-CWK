#include "videowindow.h"
#include "ui_videowindow.h"
#include <QFileDialog>
#include <QColorDialog>
#include <QStandardPaths>
#include <QDateTime>
#include <QMessageBox>



void videowindow::Volumeadjust(QString path)
{
    QSoundEffect *soundEffect = new QSoundEffect();
    soundEffect->setSource(QUrl::fromLocalFile(path));
    soundEffect->setVolume(1.0);
    soundEffect->play();
}


void videowindow::init_upper_border()
{
    upper_menu();
    upper_menu_connections();
}


QAction* createAction(const QString &text, QWidget *parent, const QIcon &icon = QIcon()) {
    return new QAction(icon, text, parent);
}

void videowindow:: upper_menu()
{
// Helper function for creating menu items
    // Create a menu bar
    menu_bar = new QMenuBar(this);
    this->setMenuBar(menu_bar);
    language_list = new QMenu("Language", menu_bar);
    background_list = new QMenu("UI color", menu_bar);
    speed_list_item = new QMenu("Speed", menu_bar);
    speed_list = new QMenu("...", menu_bar);

    // Create an action
    chinese = createAction("Chinese", this);
    english = createAction("English", this);
    custom = createAction("Choose", this);
    like = createAction("Like", this, QIcon(":/img/aixin.png"));
    picture = createAction("Screen Photo", this, QIcon(":/img/photo.png"));
    halfspeed = createAction("0.5 speed", this);
    sevenfivespeed = createAction("0.75 speed", this);
    onespeed = createAction("1.0 speed", this);
    onefivespeed = createAction("1.5 speed", this);
    twospeed = createAction("2.0 speed", this);

    // Add action to menu
    QAction* languageActions[] = {chinese, english};
    for (QAction *action : languageActions) {
        language_list->addAction(action);
        language_list->addSeparator();
    }

    background_list->addAction(custom);

    QAction* speedActions[] = {like, picture};
    for (QAction *action : speedActions) {
        speed_list->addAction(action);
        speed_list->addSeparator();
    }

    QAction* speedItemActions[] = {halfspeed, sevenfivespeed, onespeed, onefivespeed, twospeed};
    for (QAction *action : speedItemActions) {
        speed_list_item->addAction(action);
        speed_list_item->addSeparator();

    }

    menu_bar->addMenu(language_list);
    menu_bar->addMenu(speed_list_item);
    menu_bar->addMenu(background_list);
    menu_bar->addMenu(speed_list);


    menu_bar->show();

    menu_bar->setStyleSheet("QMenu {"
                            "margin:7px;"
                            "padding:3px;"
                            "color:black;"
                            "background-color:white;"
                            "border-radius:2px;"
                            "}"
                            "QMenu::separator {"
                            "margin:4px;"
                            "margin-left:8px;"
                            "margin-right:8px;"
                            "height:1px;"
                            "background:#bbbbbb;"
                            "}"
                            "QMenu::item:text {"
                            "font-size:15px;"
                            "padding-left:10px;"
                            "padding-right:10px;"
                            "}"
                            "QMenu::item:selected {"
                            "color:#1aa3ff;"
                            "background-color: #e5f5ff;"
                            "border-radius:2px;"
                            "}"
                            );


}

void videowindow:: upper_menu_connections()
{
    connect(chinese,&QAction::triggered,this,[=](){
        list->setTitle("播放列表");
        speed_list_item->setTitle("倍速播放");
        background_list->setTitle("程序皮肤");
        custom->setText("选择");
        bordervideo->setText("播放：");
        onespeed->setText("1倍速");
        halfspeed->setText("0.5倍速");
        sevenfivespeed->setText("0.75倍速");
        onefivespeed->setText("1.5倍速");
        twospeed->setText("2倍速");
        language_list->setTitle("语言");
        chinese->setText("中文（简体）");
        english->setText("英语");
        like->setText("喜欢");
        picture->setText("截图");
        list_name->setText("播放历史");
        item_list->setTitle("播放历史");
    });
    connect(english,&QAction::triggered,this,[=](){
        list->setTitle("Play List");
        speed_list_item->setTitle("Speed");
        background_list->setTitle("UI color");
        custom->setText("choose");

        bordervideo->setText("Playing：");
        onespeed->setText("1.0 speed");
        sevenfivespeed->setText("0.75 speed");
        onefivespeed->setText("1.5 speed");
        halfspeed->setText("0.5 speed");
        twospeed->setText("2.0 speed");

        language_list->setTitle("Language");
        english->setText("English");
        chinese->setText("Chinese");
        like->setText("Like");
        picture->setText("Cut screen");
        list_name->setText("Play History");
        item_list->setTitle("Play List");

    });

    connect(custom, &QAction::triggered, this, [=]() {
        QColor selectedColor = QColorDialog::getColor(Qt::white, this, tr("Select Background Color"));
        if (selectedColor.isValid()) {
            QString styleSheet = QString("background-color: %1;").arg(selectedColor.name());
            ui->mainwindow->setStyleSheet(styleSheet);
        }
    });

    connect(twospeed,&QAction::triggered,this,[=](){
        play_area->setPlaybackRate(2.0);
    });
    connect(onespeed,&QAction::triggered,this,[=](){
        play_area->setPlaybackRate(1.0);
    });
    connect(halfspeed,&QAction::triggered,this,[=](){
        play_area->setPlaybackRate(0.5);
    });
    connect(onefivespeed,&QAction::triggered,this,[=](){
        play_area->setPlaybackRate(1.5);
    });
    connect(sevenfivespeed,&QAction::triggered,this,[=](){
        play_area->setPlaybackRate(0.75);
    });

    connect(like, &QAction::triggered, this, [=]() {
        if (favoriteStatus == NotFavorite) {
            like->setIcon(QIcon(":/img/aixin2.png"));
            favoriteStatus = Favorite;
        } else {
            like->setIcon(QIcon(":/img/aixin.png"));
            favoriteStatus = NotFavorite;

        }
    });


    connect(picture,&QAction::triggered,this,[=](){
        QString defaultFileName = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)
                                  + "/CineSync Leeds pic_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");

        QString filePath = QFileDialog::getSaveFileName(
            this,
            tr("Save screenshot"),
            defaultFileName,
            tr("JPEG Image (*.jpg);;PNG Image (*.png)"));
        if (!filePath.isEmpty()) {
            QPixmap screenshot = ui->mainwindow->grab();  // Capture screenshot
            bool saveSuccess = screenshot.save(filePath, Q_NULLPTR, 100);  // Save the screenshot

            // Check if the screenshot was saved successfully
            if (saveSuccess) {
                QMessageBox::information(this, tr("Save Successful"),
                                         tr("The screenshot was saved successfully to:\n%1").arg(filePath));
            } else {
                QMessageBox::warning(this, tr("Save Failed"),
                                     tr("Failed to save the screenshot."));
            }
        }
    });

}

