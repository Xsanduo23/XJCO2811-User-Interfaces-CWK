#include "tableitem.h"
#include "ui_tableitem.h"
#include <QDebug>

tableitem::tableitem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tableitem)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: transparent;");
    ui->toolButton_cellplay->hide();
    ui->toolButton->installEventFilter(this);
    ui->toolButton->setObjectName("unlike");
    connect(ui->toolButton,&QToolButton::clicked,this,[=](){
        if(ui->toolButton->objectName()!="like")
        {
            ui->toolButton->setIcon(QIcon(":/img/celllike.png"));
            ui->toolButton->setObjectName("like");
            emit(like());
        }
        else
            {
            ui->toolButton->setIcon(QIcon(":/img/cellunlike.png"));
            ui->toolButton->setObjectName("unlike");
            emit(unlike());
        }
    });

    connect(ui->toolButton_cellplay,&QToolButton::clicked,this,[=](){
        emit(playmusic());
    });
}

bool tableitem::eventFilter(QObject *object, QEvent *event)
{
    if (object == ui->toolButton)
    {
        if (event->type() == QEvent::Enter)
        {
            // 鼠标进入按钮区域，进行缩放
            if(ui->toolButton->objectName()!="like"){
                ui->toolButton->setIcon(QIcon(":/img/celltolike.png"));
            }
            ui->toolButton->setIconSize(QSize(30,30));

        }
        else if (event->type() == QEvent::Leave)
        {
            if(ui->toolButton->objectName()!="like"){
                ui->toolButton->setIcon(QIcon(":/img/cellunlike.png"));
            }
            else {
                ui->toolButton->setIcon(QIcon(":/img/celllike.png"));
            }
            // 鼠标离开按钮区域，恢复原始大小
            ui->toolButton->setIconSize(QSize(20, 20));
        }
    }

    return QWidget::eventFilter(object, event);
}

void tableitem::enterEvent(QEvent *event)
{
    if(this->objectName()=="movelisttableitem")
    {
        ui->toolButton_cellplay->show();
    }
    else
    {
        ui->toolButton_cellplay->show();
    }
}

void tableitem::leaveEvent(QEvent *event)
{
    if(this->objectName()=="movelisttableitem")
    {
        ui->toolButton_cellplay->hide();
    }
    else
    {
        ui->toolButton_cellplay->hide();
    }
}

int tableitem::getY() const
{
    return y;
}

void tableitem::setY(int newY)
{
    y = newY;
}

void tableitem::settoolbtnislikesetObjectname(QString name)
{
    ui->toolButton->setObjectName(name);
}

void tableitem::setthisName(QString thisname)
{
    this->setObjectName(thisname);
}

int tableitem::getX() const
{
    return x;
}

void tableitem::setX(int newX)
{
    x = newX;
}

tableitem::~tableitem()
{
    delete ui;
}

QString tableitem::getItemtext()
{
    return ui->label->text();
}

void tableitem::setItemtext(QString text)
{
    ui->label->setText(text);
}

void tableitem::settoolIcon(QIcon icon)
{
    ui->toolButton->setIcon(icon);
    cellicon = icon;
}

void tableitem::selikeiconSize(QSize size)
{
    ui->toolButton->setIconSize(size);
}
