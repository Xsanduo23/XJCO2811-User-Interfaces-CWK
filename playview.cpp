#include "playview.h"
#include "ui_playview.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QTimer>
#include <qDebug>
#include <QFile>
#include <QDir>
#include <QFileDialog>

playView::playView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::playView)
{
    ui->setupUi(this);
    //初始化播放设备
    initplayer();

    //初始化界面
    initUI();

    updataslider = new QTimer();
    connect(updataslider,&QTimer::timeout,this,[=](){
        try {
            if(myplayer->duration())
                playslider->setValue(int(myplayer->position() * 500 / myplayer->duration()));
        } catch (const std::exception& e) {
            // 处理异常情况，例如输出错误信息或进行错误处理
        }
    });

    ui->widget_2->setFixedHeight(100);

    //设置开始进度条不可拖动
    playslider->setEnabled(false);

    //设置初始音量
    voicevalue = 20;
}

playView::~playView()
{
    delete ui;
}

void playView::initUI()
{
    controlwidget = new QWidget(ui->widget_2);

    open = new QPushButton();
    initbtnicon(open,QSize(this->width()*0.05,this->width()*0.05),":/img/open.png");

    stop_start = new QPushButton();
    initbtnicon(stop_start,QSize(this->width()*0.062,this->width()*0.062),":/img/stop.png");
    howplay = new QPushButton("");
    initbtnicon(howplay,QSize(15,15),"");
    howplay->setStyleSheet("");
    back = new QPushButton();
    initbtnicon(back,QSize(this->width()*0.06,this->width()*0.06),":/img/back.png");
    advance = new QPushButton();
    initbtnicon(advance,QSize(this->width()*0.06,this->width()*0.06),":/img/advance.png");
    voice = new QPushButton();
    voice->setObjectName("yes");
    initbtnicon(voice,QSize(this->width()*0.05,this->width()*0.05),":/img/voice.png");
    localbtn = new QPushButton();
    localbtn->setObjectName("yes");
    initbtnicon(localbtn,QSize(this->width()*0.05,this->width()*0.05),":/img/locallist.png");

    voiceslider = new QSlider(this);
    voiceslider->resize(15,10);
    voiceslider->setMaximum(100);
    voiceslider->setPageStep(10);
    voiceslider->setOrientation(Qt::Horizontal);
    voiceslider->setValue(20);

    QHBoxLayout *hlayout = new QHBoxLayout();
    QSpacerItem *spacer1 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    QSpacerItem *spacer2 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);


    hlayout->addWidget(open);
    hlayout->addWidget(howplay);
    hlayout->addSpacerItem(spacer1);
    hlayout->addWidget(back);
    hlayout->addWidget(stop_start);
    hlayout->addWidget(advance);
    hlayout->addSpacerItem(spacer2);
    hlayout->addWidget(voice);
    hlayout->addWidget(voiceslider);
    hlayout->addWidget(localbtn);

    hlayout->setStretch(0, 1);
    hlayout->setStretch(1, 2);
    hlayout->setStretch(2, 5);
    hlayout->setStretch(3, 1);
    hlayout->setStretch(4, 1);
    hlayout->setStretch(5, 1);
    hlayout->setStretch(6, 4);
    hlayout->setStretch(7, 1);
    hlayout->setStretch(8, 4);
    hlayout->setStretch(9, 1);
    hlayout->setSpacing(0);

    QVBoxLayout *vlayout = new QVBoxLayout(controlwidget);

    playslider = new QSlider();
    playslider->setMaximum(500);
    playslider->setPageStep(10);
    playslider->setOrientation(Qt::Horizontal);

    QFile file(":/skin/slider.qss");
    if(file.open(QFile::ReadOnly))
    {
        QTextStream filetext(&file);
        QString stylesheet = filetext.readAll();
        playslider->setStyleSheet(stylesheet);
        voiceslider->setStyleSheet(stylesheet);
        file.close();
    }
    else
    {
        qDebug()<<"Open file fail ";
    }

    vlayout->addWidget(playslider);
    vlayout->addLayout(hlayout);

    controlwidget->resize(ui->widget_2->size());

    //滑动窗口
    initMovewidget();

    controlwidget->show();
    //事件
    connect(open,&QPushButton::clicked,this,[=](){
        singer.clear();
        QString directory = QFileDialog::getExistingDirectory(
            nullptr,
            "选择文件夹",
            QDir::homePath(),
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

        if (!directory.isEmpty())
        {
            qDebug() << "选择的文件夹路径：" << directory;
            searchMp4Files(directory);
            setdataTotable(ui->tableWidget);
        }
        qDebug()<<singer;
    });
    connect(stop_start,&QPushButton::clicked,this,[=](){
        if(myplayer->state() == QMediaPlayer::PlayingState)
            {
            initbtnicon(stop_start,QSize(stop_start->iconSize()),":/img/start.png");
            myplayer->pause();
        }
        else{
            initbtnicon(stop_start,QSize(stop_start->iconSize()),":/img/stop.png");
            myplayer->play();
        }
    });
    connect(playslider,&QSlider::sliderMoved,this,[=](){
        if(updataslider!=nullptr){
            updataslider->stop();
        }
        myplayer->setPosition(playslider->value()*myplayer->duration()/500);
    });
    connect(playslider, &QSlider::sliderReleased, this, [=](){
        updataslider->start(200);
    });

    connect(voice,&QPushButton::clicked,this,[=](){
        if(voice->objectName()=="yes")
            {
            voice->setObjectName("no");
            voiceslider->setValue(0);
            initbtnicon(voice,QSize(voice->iconSize()), ":/img/novoice.png");
        }
        else{
            voice->setObjectName("yes");
            voiceslider->setValue(voicevalue);
            myplayer->setVolume(voicevalue);
            initbtnicon(voice, QSize(voice->iconSize()), ":/img/voice.png");
        }
    });

    connect(voiceslider,&QSlider::valueChanged,this,[=](int value){
        myplayer->setVolume(value);
        if(value!=0)
            voicevalue = value;
    });

    connect(localbtn,&QPushButton::clicked,this,[=](){
        if(!ui->tableWidget->isVisible())
            {
            movewidget->show();
            m_propertyAnimation->setStartValue(QRect(this->rect().width(),0,movewidget->width(),movewidget->height()));
            m_propertyAnimation->setEndValue(QRect(this->rect().width()-movewidget->width(),0,movewidget->width(),movewidget->height()));
            m_propertyAnimation->start();
            ishide = false;
        }
    });

    connect(back,&QPushButton::clicked,this,[=](){
        updataslider->stop();
        myplayerlist->previous();
        myplayer->play();
        updataslider->start(200);
    });
    connect(advance,&QPushButton::clicked,this,[=](){
        updataslider->stop();
        myplayerlist->next();
        myplayer->play();
        updataslider->start(200);
    });

    connect(howplay,&QPushButton::clicked,this,[=](){
        if(howplay->text()=="")
        {
        }
        else
        {
        }
    });
}

void playView::initplayer()
{
    myplayer = new QMediaPlayer();
    myplayerlist = new QMediaPlaylist();

    // 设置播放模式为循环播放
    myplayerlist->setPlaybackMode(QMediaPlaylist::Loop);

    //事件
    connect(myplayerlist,&QMediaPlaylist::currentIndexChanged,this,[=](int index){
        QMediaContent currentMedia = myplayerlist->currentMedia();
        QString currentPlayerName = currentMedia.canonicalUrl().fileName();
        if(currentPlayerName.count("mp3"))
        {
            ui->widget->hide();
        }
        else {
            ui->widget->show();
        }
    });

    myvideowidget = new QVideoWidget(ui->widget);
    myplayer->setPlaylist(myplayerlist);
    myplayer->setVideoOutput(myvideowidget);
    myvideowidget->resize(ui->widget->size());

    myplayer->setVolume(20);
}

void playView::initMovewidget()
{
    movewidget = new QWidget(this);
    movewidget->resize(this->width()*0.33,this->height());
    movewidget->setMaximumWidth(400);
    movewidget->setStyleSheet("background-color: rgb(41, 41, 43);");
    movewidget->hide();

    m_propertyAnimation = new QPropertyAnimation(movewidget,"geometry");
    m_propertyAnimation->setEasingCurve(QEasingCurve::InOutSine);
    m_propertyAnimation->setDuration(150);

    QLabel *movetitle = new QLabel("Play List");
    movetitle->setStyleSheet("color: rgb(255, 255, 255); font: 700 20pt Microsoft YaHei UI");

    listtable = new mytable();
    connect(listtable,&QTableWidget::cellDoubleClicked,this,[=](int i,int j){
        // 获取第0行第0列的单元格的部件
        QWidget *widget = listtable->cellWidget(i, j);
        if (widget) {
            // 将获取的部件转换为 tableitem 类型，并获取 label 的文本
            tableitem *item = qobject_cast<tableitem *>(widget);
            if (item) {
                QString text = item->getItemtext();
                // 对 label 文本进行处理
                this->setsrc(text);
            }
        } else {
            // 没有存储的部件
        }
    });

    QVBoxLayout *movelayout = new QVBoxLayout(movewidget);

    movelayout->addWidget(movetitle);
    movelayout->addWidget(listtable);
    movelayout->setStretch(0,1);
    movelayout->setStretch(1,8);

}

void playView::initbtnicon(QPushButton *btn, QSize size, QString iconpath)
{
    btn->setFlat(true);
    btn->resize(size);
    btn->setIcon(QIcon(iconpath));
    btn->setIconSize(btn->size());
}

void playView::setsrc(QString path)
{
    // 遍历播放列表查找媒体文件
    bool hasFile = false;
    int current = 0;
    for (int i = 0; i < myplayerlist->mediaCount(); ++i) {
        QMediaContent media = myplayerlist->media(i);
        QUrl url = media.canonicalUrl();
        QString fileName = url.fileName();
        if (fileName == QUrl(path).fileName()) {
            hasFile = true;
            current = i;
            break;
        }
    }

    if (hasFile) {
        myplayerlist->setCurrentIndex(current);
        myplayer->play();
    } else {
        myplayerlist->addMedia(QUrl(path));
        myplayerlist->setCurrentIndex(myplayerlist->mediaCount() - 1);
        myplayer->play();
        insertdateTotable(path);
    }

    //当播放时设置进度条可用
    if(playslider->isEnabled()==false)
        playslider->setEnabled(true);

    // 设置循环播放
    updataslider->start(200);
    if(path.count("mp3"))
    {
        ui->widget->hide();
    }
    else {
        ui->widget->show();
    }
}

void playView::insertdateTotable(QString name)
{
    int row  = listtable->rowCount();
    listtable->setColumnCount(1); // 在循环之前先设置表格的列数
    listtable->insertRow(row);
    tableitem *item = new tableitem();
    item->setthisName("movelisttableitem");
    item->setItemtext(name);
    item->selikeiconSize(QSize(20,20));
    item->resize(listtable->width(),listtable->columnWidth(row));
    listtable->setCellWidget(row,0,item);
    QVector<QVariantList>data;
    if(data.size())
    {
        if(data.at(0).at(0).toString()=="unlike"){
            item->settoolIcon(QIcon(":/img/cellunlike.png"));
            item->settoolbtnislikesetObjectname("unlike");
        }
        else{
            item->settoolIcon(QIcon(":/img/celllike.png"));
            item->settoolbtnislikesetObjectname("like");
        }
    }

    //播放按钮消息
    connect(item,&tableitem::playmusic,this,[=](){this->setsrc(item->getItemtext());});
}

void playView::searchMp4Files(const QString &directory) {
    QDir dir(directory);
    QStringList filters;

// 使用预处理器指令来区分不同的操作系统
#if defined(_WIN32)
    filters << "*.wmv";
#else
    filters << "*.mp4" << "*.mov";
#endif

    QStringList files = dir.entryList(filters, QDir::Files);
    foreach (const QString &file, files) {
        QUrl fileUrl = QUrl::fromLocalFile(dir.filePath(file));
        qDebug() << dir.filePath(file);
        singer.push_back(fileUrl.toString());
    }
}

void playView::setdataTotable(QTableWidget *table)
{
    table->setRowCount(0);
    table->setColumnCount(1); // 在循环之前先设置表格的列数
    for(int i = 0; i < singer.size(); i++)
    {
        table->insertRow(i);
        tableitem *item = new tableitem();
        item->settoolIcon(QIcon(":/img/cellunlike.png"));
        item->setItemtext(singer.at(i));
        item->selikeiconSize(QSize(20,20));
        item->setX(i);
        item->setY(0);
        table->setCellWidget(i,0,item);
        item->resize(table->width(),table->columnWidth(i));
        //播放按钮消息
        connect(item,&tableitem::playmusic,this,[=](){this->setsrc(item->getItemtext());});
    }
}

void playView::resizeBtn()
{
    if(ui->tableWidget->isVisible())
    {
        open->setIconSize(QSize(this->width()*0.05,this->width()*0.05));
        localbtn->setIconSize(QSize(this->width()*0.05,this->width()*0.05));
        howplay->setIconSize(QSize(this->width()*0.05,this->width()*0.05));
        voice->setIconSize(QSize(this->width()*0.05,this->width()*0.05));
        stop_start->setIconSize(QSize(this->width()*0.075,this->width()*0.075));
        advance->setIconSize(QSize(this->width()*0.06,this->width()*0.06));
        back->setIconSize(QSize(this->width()*0.06,this->width()*0.06));
    }
}

void playView::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    myvideowidget->resize(ui->widget->size());    //设置播放界面大小
    controlwidget->resize(ui->widget_2->size());

    movewidget->resize(this->width()*0.3,this->height());
    movewidget->hide();

    //PC和手机切换
    if(this->width()>this->height())
        ui->tableWidget->hide();
    else
        ui->tableWidget->show();

    resizeBtn();
}

void playView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if(ui->widget_2->isVisible())
        {
            ui->widget_2->hide();
            ui->tableWidget->hide();
            this->showFullScreen();
        }
        else
        {
            this->showNormal();
            ui->widget_2->show();
            if(this->width()<this->height())
                ui->tableWidget->show();
            this->resize(this->width(),this->height()+1);
            this->resize(this->width(),this->height()-1);
        }
    }

    // 调用基类的实现以保持其他默认行为
    QWidget::mouseDoubleClickEvent(event);
}

void playView::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
}

void playView::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
}

void playView::mousePressEvent(QMouseEvent *event)
{
    if(!ishide)
    {
        m_propertyAnimation->setStartValue(QRect(this->rect().width()-movewidget->width(),0,movewidget->width(),movewidget->height()));
        m_propertyAnimation->setEndValue(QRect(this->rect().width(),0,movewidget->width(),movewidget->height()));
        m_propertyAnimation->start();
        ishide = true;
    }
}
