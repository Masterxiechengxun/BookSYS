#include "fonts.h"

#include "poppler-qt5.h"

#include <QtWidgets/QTableWidget>

static QString yesNoStatement(bool value)
{
    return value ? QString::fromLatin1("yes") : QString::fromLatin1("no");
}

FontsDock::FontsDock(QWidget *parent)
    : AbstractInfoDock(parent)
{
    m_table = new QTableWidget(this);
    setWidget(m_table);
    setWindowTitle(tr("Fonts"));
    m_table->setColumnCount(5);
    m_table->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("Type") << tr("Embedded") << tr("Subset") << tr("File"));
    m_table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
}

FontsDock::~FontsDock()
{
}

void FontsDock::fillInfo()
{
    const QList<Poppler::FontInfo> fonts = document()->fonts();
    m_table->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("Type") << tr("Embedded") << tr("Subset") << tr("File"));
    m_table->setRowCount(fonts.count());
    int i = 0;
    Q_FOREACH(const Poppler::FontInfo &font, fonts) {
        if (font.name().isNull()) {
            m_table->setItem(i, 0, new QTableWidgetItem(QString::fromLatin1("[none]")));
        } else {
            m_table->setItem(i, 0, new QTableWidgetItem(font.name()));
        }
        m_table->setItem(i, 1, new QTableWidgetItem(font.typeName()));
        m_table->setItem(i, 2, new QTableWidgetItem(yesNoStatement(font.isEmbedded())));
        m_table->setItem(i, 3, new QTableWidgetItem(yesNoStatement(font.isSubset())));
        m_table->setItem(i, 4, new QTableWidgetItem(font.file()));
        ++i;
    }
}

void FontsDock::documentClosed()
{
    m_table->clear();
    m_table->setRowCount(0);
    AbstractInfoDock::documentClosed();
}
