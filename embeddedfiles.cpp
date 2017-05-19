#include "embeddedfiles.h"

#include "poppler-qt5.h"

#include <QtWidgets/QTableWidget>

EmbeddedFilesDock::EmbeddedFilesDock(QWidget *parent)
    : AbstractInfoDock(parent)
{
    m_table = new QTableWidget(this);
    setWidget(m_table);
    setWindowTitle(tr("Embedded files"));
    m_table->setColumnCount(6);
    m_table->setHorizontalHeaderLabels(
        QStringList() << tr("Name") << tr("Description") << tr("Size") << tr("Creation date")
                      << tr("Modification date") << tr("Checksum"));
    m_table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
}

EmbeddedFilesDock::~EmbeddedFilesDock()
{
}

void EmbeddedFilesDock::fillInfo()
{
    m_table->setHorizontalHeaderLabels(
        QStringList() << tr("Name") << tr("Description") << tr("Size") << tr("Creation date")
                      << tr("Modification date") << tr("Checksum"));
    if (!document()->hasEmbeddedFiles()) {
        m_table->setItem(0, 0, new QTableWidgetItem(tr("No files")));
        return;
    }

    const QList<Poppler::EmbeddedFile*> files = document()->embeddedFiles();
    m_table->setRowCount(files.count());
    int i = 0;
    Q_FOREACH(Poppler::EmbeddedFile *file, files) {
        m_table->setItem(i, 0, new QTableWidgetItem(file->name()));
        m_table->setItem(i, 1, new QTableWidgetItem(file->description()));
        m_table->setItem(i, 2, new QTableWidgetItem(QString::number(file->size())));
        m_table->setItem(i, 3, new QTableWidgetItem(file->createDate().toString(Qt::SystemLocaleDate)));
        m_table->setItem(i, 4, new QTableWidgetItem(file->modDate().toString(Qt::SystemLocaleDate)));
        const QByteArray checksum = file->checksum();
        const QString checksumString = !checksum.isEmpty() ? QString::fromLatin1(checksum.toHex()) : QString::fromLatin1("n/a");
        m_table->setItem(i, 5, new QTableWidgetItem(checksumString));
        ++i;
    }
}

void EmbeddedFilesDock::documentLoaded()
{
    if ( document()->pageMode() == Poppler::Document::UseAttach ) {
        show();
    }
}

void EmbeddedFilesDock::documentClosed()
{
    m_table->clear();
    m_table->setRowCount(0);
    AbstractInfoDock::documentClosed();
}
