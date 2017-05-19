#include "metadata.h"

#include "poppler-qt5.h"

#include <QtWidgets/QTextEdit>

MetadataDock::MetadataDock(QWidget *parent)
    : AbstractInfoDock(parent)
{
    m_edit = new QTextEdit(this);
    setWidget(m_edit);
    setWindowTitle(tr("Metadata"));
    m_edit->setAcceptRichText(false);
    m_edit->setReadOnly(true);
}

MetadataDock::~MetadataDock()
{
}

void MetadataDock::fillInfo()
{
    m_edit->setPlainText(document()->metadata());
}

void MetadataDock::documentClosed()
{
    m_edit->clear();
    AbstractInfoDock::documentClosed();
}
