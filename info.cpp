#include "info.h"

#include "poppler-qt5.h"

#include <QtWidgets/QTableWidget>

InfoDock::InfoDock(QWidget *parent)
    : AbstractInfoDock(parent)
{
    m_table = new QTableWidget(this);
    setWidget(m_table);
    setWindowTitle(tr("Information"));
    m_table->setColumnCount(2);
    m_table->setHorizontalHeaderLabels(QStringList() << tr("Key") << tr("Value"));
    m_table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
}

InfoDock::~InfoDock()
{
}

void InfoDock::fillInfo()
{
    QStringList keys = document()->infoKeys();
    m_table->setHorizontalHeaderLabels(QStringList() << tr("Key") << tr("Value"));
    m_table->setRowCount(keys.count());
    QStringList dateKeys;
    dateKeys << QString::fromLatin1("CreationDate");
    dateKeys << QString::fromLatin1("ModDate");
    int i = 0;
    Q_FOREACH(const QString &date, dateKeys) {
        const int id = keys.indexOf(date);
        if (id != -1) {
            m_table->setItem(i, 0, new QTableWidgetItem(date));
            m_table->setItem(i, 1, new QTableWidgetItem(document()->date(date).toString(Qt::SystemLocaleDate)));
            ++i;
            keys.removeAt(id);
        }
    }
    Q_FOREACH(const QString &key, keys) {
        m_table->setItem(i, 0, new QTableWidgetItem(key));
        m_table->setItem(i, 1, new QTableWidgetItem(document()->info(key)));
        ++i;
    }
}

void InfoDock::documentClosed()
{
    m_table->clear();
    m_table->setRowCount(0);
    AbstractInfoDock::documentClosed();
}
