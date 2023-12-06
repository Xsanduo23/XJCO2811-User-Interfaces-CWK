#include "widget.h"
#include "ui_widget.h"
#include <QTest>
#include <QThread>
#include <QMessageBox>

#include <QDebug>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->horizontalSlider->setEnabled(true);
    ui->horizontalSlider->setRange(0,100);
    connect(&my_timer,SIGNAL(timeout()),this,SLOT(onTimeOut()));  //绑定定时器，实时适应屏幕大小
    connect(&playbar,SIGNAL(timeout()),this,SLOT(onTimeOutBar()));
    connect(ui->horizontalSlider_2,SIGNAL(valueChanged(int)),this,SLOT(ChangedVoice(int)));
//    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(ChangedVoiceBar(int)));  //不能用，会乱
    start = true;
    sound = true;
    setButtonImage(ui->pushButton_start_stop, ":/start.png",60,60);
    setButtonImage(ui->pushButton_sound, ":/sound.png",38,38);

    //播放者
    myplayer = new QMediaPlayer;
    //播放列表
    myplayerlist = new QMediaPlaylist;
    //在哪播放
    mywidget = new QVideoWidget(ui->graphicsView);

    myplayerlist->setPlaybackMode(QMediaPlaylist::Loop);// 通过设置 播放列表循环 实现循环播放
    myplayer->setPlaylist(myplayerlist);
    myplayer->setVideoOutput(mywidget);
    mywidget->resize(ui->graphicsView->size());    //设置播放界面大小
    setWindowTitle("CineSync");          //设置界面名称

    myplayer->setVolume(50);                     //设置初始音量为50
    ui->horizontalSlider_2->setValue(50);

    this->setStyleSheet("background-color:rgb(5,7,22)");
    my_timer.start(10);
    //*********************************************样式设置*****************************************//
    QString sliderstyle = QString("QSlider::groove:horizontal{border:none; height:12px; border-radius:6px; background:rgba(0, 219, 39, 100);}"

                                  "QSlider::handle:horizontal{border:none; margin:-5px 0px; width:22px; height:22px; border-radius:20px;"
                                                             "border-image:url(:/stop.png);}"

                                 "QSlider::sub-page:horizontal{background:rgba(0, 150, 39, 100); border-radius:6px;}"

                                 "QSlider::add-page:horizontal{background:rgba(0, 19, 39, 100); border-radius:6px;}");
    QString sliderstyle2 = QString("QSlider::groove:horizontal{border:none; height:12px; border-radius:6px; background:rgba(0, 219, 39, 100);}"

                                  "QSlider::handle:horizontal{border:none; margin:-5px 0px; width:22px; height:22px; border-radius:20px;"
                                                             "border-image:url(:/sound.png);}"

                                 "QSlider::sub-page:horizontal{background:rgba(0, 150, 39, 100); border-radius:6px;}"

                                 "QSlider::add-page:horizontal{background:rgba(0, 19, 39, 100); border-radius:6px;}");
    ui->horizontalSlider->setStyleSheet(sliderstyle);
    ui->horizontalSlider_2->setStyleSheet(sliderstyle2);
}

Widget::~Widget()
{
    delete ui;
}

//设置按钮图标，按钮的默认大小是 30*30，可以自己指定
void Widget:: setButtonImage(QPushButton *button, QString image,int xsize,int ysize)
{
    button->setText("");
    QPixmap pixmap(image);
    QPixmap fitpixmap = pixmap.scaled(xsize, ysize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    button->setIcon(QIcon(fitpixmap));
    button->setIconSize(QSize(xsize, ysize));
    button->setFlat(true);
    button->setStyleSheet("border: 0px"); //消除边框
}

void Widget::on_pushButton_start_stop_clicked()
{
    if(start)
    {
        setButtonImage(ui->pushButton_start_stop, ":/stop.png",60,60);
        myplayer->play();
//        QTest::qSleep(1000); //暂停1秒
        playbar.start(1000); //定时器控制视频进度条跟新

        start = false;
    }

    else
    {
        setButtonImage(ui->pushButton_start_stop, ":/start.png",60,60);
        myplayer->pause();
//        playbar.stop();
        start = true;
    }
}

void Widget::on_pushButton_choice_clicked()
{
    //打开文件
    QStringList mylist = QFileDialog::getOpenFileNames(this,"Choice",                         //弹出"选择播放路径"窗口
                                                       "C:",    //播放路径
                                                        "allfiles(*.*);;"                          //所以文件类型
                                                        "MP3(*.mp3);;"                             //音频
                                                        "MP4(*.mp4);;");                           //视频
    myplayerlist->clear();
    for(const auto &k:mylist)
        myplayerlist->addMedia(QUrl::fromLocalFile(k));

//    myplayer->play();
    myplayer->play();
    QTest::qSleep(1);
    myplayer->pause();  //让界面有初始页面
    mywidget->resize(ui->graphicsView->size());    //设置播放界面大小
    setButtonImage(ui->pushButton_start_stop, ":/start.png",60,60);
}

void Widget:: onTimeOut()
{
    mywidget->resize(ui->graphicsView->size());    //设置播放界面大小
}

void Widget:: onTimeOutBar()
{
    if(myplayer->duration()!=0)
        ui->horizontalSlider->setValue(int(myplayer->position()*100/myplayer->duration()));
}

void Widget::ChangedVoice(int value)   //进度条自动将值传为形参
{
    myplayer->setVolume(value);     //设置进度条连接槽，改变音量
//    qDebug()<<"voice:"<<QString::number(value);
    setButtonImage(ui->pushButton_sound, ":/sound.png",38,38);
    sound = true;
//    qDebug()<<"local:"<<QString::number(myplayer->position())<<"all:"<<QString::number(myplayer->duration());
}

void Widget::on_pushButton_sound_clicked()
{
    if(sound)
    {
        setButtonImage(ui->pushButton_sound, ":/soundclose.jpg",38,38);
        myplayer->setVolume(0);      //设置静音
        sound = false;
    }
    else
    {
        setButtonImage(ui->pushButton_sound, ":/sound.png",38,38);
        ChangedVoice(ui->horizontalSlider_2->value());   //重新开启声音
        sound = true;
    }
//    playbar.start(1000);
//    ui->horizontalSlider->setValue(int(myplayer->position()*100/myplayer->duration()));
}

//void Widget::ChangedVoiceBar(int value)
//{
//    myplayer->setPosition((value)*myplayer->duration()/100);
//}

void Widget::on_horizontalSlider_sliderMoved(int position)
{
    //暂时停止计时器，在用户拖动过程中不修改slider的值
    playbar.stop();
    myplayer->setPosition(ui->horizontalSlider->value()*myplayer->duration()/100);
}

void Widget::on_horizontalSlider_sliderReleased()
{
    playbar.start(1000);
}

void Widget::on_pushButton_kuaijin_clicked()
{
    if((ui->horizontalSlider->value()+10)>=100)
        myplayer->setPosition(0);
    else
    myplayer->setPosition((ui->horizontalSlider->value()+10)*myplayer->duration()/100);
}

void Widget::on_pushButton_houtui_clicked()
{
    if((ui->horizontalSlider->value()-10)<=0)
        myplayer->setPosition(0);
    else
    myplayer->setPosition((ui->horizontalSlider->value()-10)*myplayer->duration()/100);
}

void Widget:: AllHide()
{
//    ui->horizontalSlider->hide();
    ui->horizontalSlider_2->hide();
    ui->pushButton_sound->hide();
    ui->pushButton_choice->hide();
    ui->pushButton_kuaijin->hide();
    ui->pushButton_start_stop->hide();
    ui->pushButton_houtui->hide();
    ui->line->hide();
//    ui->graphicsView->setWindowFlags(Qt::Window);
//    ui->graphicsView->showFullScreen();

//    showFullScreen(); // Qt全屏显示函数
}

void Widget:: AllShow()
{
//    ui->horizontalSlider->show();
    ui->horizontalSlider_2->show();
    ui->pushButton_sound->show();
    ui->pushButton_choice->show();
    ui->pushButton_kuaijin->show();
    ui->pushButton_start_stop->show();
    ui->pushButton_houtui->show();
    ui->line->show();
}
void Widget:: enterEvent(QEvent *e)
{
    AllShow();
}

void Widget::leaveEvent(QEvent *e)
{
    AllHide();
}

