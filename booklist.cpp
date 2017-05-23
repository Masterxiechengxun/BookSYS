#include "booklist.h"

BookList::BookList(QWidget *parent) :
    QListView(parent)
{
    conn = connService::getService();
    conn->dataWrite("booklist admin");
    QString result = static_cast<QString>(conn->getBuf()).toUtf8();
    QStringList list = result.split('#');
    list.removeLast();
    list.sort();
    model = new QStandardItemModel(list.size(), 1);
    for(int row = 0; row < list.size(); row++)
        model->setItem(row, new QStandardItem(list.at(row)));
    this->setModel(model);
    this->show();
}

BookList::~BookList()
{
    if(model)
        delete model;
}

void BookList::setPWD(QString p)
{
    pwd = p;
}

void BookList::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        emit onReceiveBook();
        this->setEnabled(false);
        QModelIndex index = this->currentIndex();
        bookTitle = index.data().toString();
        if(bookTitle == "")
            return;
        conn = connService::getService();
        //QString pathName = "C:\\pdfsys\\localFiles\\" + bookTitle;
        //QFile file(pathName);
        conn->setPWD(pwd);
        conn->dataWrite(tr("read #%1#%2").arg(bookTitle).arg(pwd));
    }
}
