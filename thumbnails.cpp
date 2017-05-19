#include "thumbnails.h"

#include "poppler-qt5.h"

#include <QtWidgets/QListWidget>

static const int PageRole = Qt::UserRole + 1;

ThumbnailsDock::ThumbnailsDock(QWidget *parent)
    : AbstractInfoDock(parent)
{
    m_list = new QListWidget(this);
    setWidget(m_list);
    setWindowTitle(tr("Thumbnails"));
    m_list->setViewMode(QListView::ListMode);
    m_list->setMovement(QListView::Static);
    m_list->setVerticalScrollMode(QListView::ScrollPerPixel);
    connect(m_list, SIGNAL(itemActivated(QListWidgetItem*)),
            this, SLOT(slotItemActivated(QListWidgetItem*)));
}

ThumbnailsDock::~ThumbnailsDock()
{
}

void ThumbnailsDock::fillInfo()
{
    const int num = document()->numPages();
    QSize maxSize;
    for (int i = 0; i < num; ++i) {
        const Poppler::Page *page = document()->page(i);
        const QImage image = page->thumbnail();
        if (!image.isNull()) {
            QListWidgetItem *item = new QListWidgetItem();
            item->setText(QString::number(i + 1));
            item->setData(Qt::DecorationRole, QPixmap::fromImage(image));
            item->setData(PageRole, i);
            m_list->addItem(item);
            maxSize.setWidth(qMax(maxSize.width(), image.width()));
            maxSize.setHeight(qMax(maxSize.height(), image.height()));
        }
        delete page;
    }
    if (num > 0) {
        m_list->setGridSize(maxSize);
        m_list->setIconSize(maxSize);
    }
}

void ThumbnailsDock::documentClosed()
{
    m_list->clear();
    AbstractInfoDock::documentClosed();
}

void ThumbnailsDock::slotItemActivated(QListWidgetItem *item)
{
    if (!item) {
        return;
    }

    setPage(item->data(PageRole).toInt());
}
