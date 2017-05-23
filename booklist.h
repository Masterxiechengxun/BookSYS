#ifndef BOOKLIST_H
#define BOOKLIST_H

#include <QListView>
#include <QWidget>
#include <QList>
#include <QStandardItem>
#include <QDebug>
#include <QMouseEvent>
#include <QStringList>
#include <QFont>
#include <QFile>
#include <QTime>
#include <QString>

#include "connservice.h"


class BookList : public QListView
{
    Q_OBJECT
public:
    static BookList *getBookList(QWidget *parent = 0)
    {
        static BookList instance(parent);
        return &instance;

    }
    void mouseDoubleClickEvent(QMouseEvent *event);
    void setPWD(QString p);

signals:
    void onReceiveBook();
    void onSelectBook(QString pathName);

private:
    QStandardItemModel *model;
    QString bookTitle;
    connService *conn;
    BookList(QWidget *parent = 0);
    ~BookList();

    QString pwd;
};

#endif // BOOKLIST_H
