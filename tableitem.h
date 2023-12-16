#ifndef TABLEITEM_H
#define TABLEITEM_H

#include <QWidget>
#include <QIcon>

namespace Ui {
class tableitem;
}

class tableitem : public QWidget
{
    Q_OBJECT

public:
    explicit tableitem(QWidget *parent = nullptr);
//    explicit tableitem(QWidget *parent = nullptr,QString icon = nullptr,QString text = nullptr);
    ~tableitem();
    QString getItemtext();
    void setItemtext(QString text);
    void settoolIcon(QIcon icon);
    void selikeiconSize(QSize size);

    int getX() const;
    void setX(int newX);

    int getY() const;
    void setY(int newY);

    void settoolbtnislikesetObjectname(QString name);
    void setthisName(QString thisname);

protected:
    bool eventFilter(QObject *object, QEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;

signals:
    void unlike();
    void like();
    void playmusic();
    void setcellstyle();
    void addtolist(QString name);
    void movefromlist(QString name);
    void delthismusic(QString name);


private:
    Ui::tableitem *ui;
    QIcon cellicon;
    int x,y;
};

#endif // TABLEITEM_H
