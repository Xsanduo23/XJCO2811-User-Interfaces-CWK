#include "mytable.h"
#include <QDebug>
#include <QFile>
#include <QHeaderView>

mytable::mytable(QWidget *parent)
    : QTableWidget{parent}
{
    this->setColumnCount(1);
    QStringList header;
    header<<"Play List";
    this->setHorizontalHeaderLabels(header);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->setMinimumSize(10,10);
    this->verticalHeader()->setVisible(false); //不显示行号
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setMouseTracking(true);
    // 设置表格字体颜色为白色
    this->setStyleSheet("color: white");
    this->horizontalHeader()->setStyleSheet("background-color: transparent; color: red;");
    QFile file(":/skin/table.qss");
    if(file.open(QFile::ReadOnly))
    {
        QTextStream filetext(&file);
        QString stylesheet = filetext.readAll();
        this->setStyleSheet(stylesheet);
        file.close();
    }
    else
    {
        qDebug()<<"Open file fail ";
    }
}
