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
    //model->setHeaderData(0, Qt::Horizontal, "books on the server");
    for(int row = 0; row < list.size(); row++)
        model->setItem(row, new QStandardItem(list.at(row)));
    this->setModel(model);
    //this->expandAll();
    this->show();
}

BookList::~BookList()
{
    if(model)
        delete model;
}

void BookList::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        emit onReceiveBook();
        this->setEnabled(false);
        QModelIndex index = this->currentIndex();
        bookTitle = index.data().toString();
        conn = connService::getService();
        QString pathName = "C:\\pdfsys\\localFiles\\" + bookTitle;
        QFile file(pathName);
        if(!file.exists())
        {
            conn->dataWrite(tr("read #%1").arg(bookTitle));
        }
        else
        {
            emit onSelectBook(pathName);
        }

    }
}
