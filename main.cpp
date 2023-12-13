#include "playview.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    playView *p = new playView();
    p->show();
    return a.exec();
}
