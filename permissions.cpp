#include "permissions.h"

#include "poppler-qt5.h"

#include <QtWidgets/QListWidget>

PermissionsDock::PermissionsDock(QWidget *parent)
    : AbstractInfoDock(parent)
{
    m_table = new QListWidget(this);
    setWidget(m_table);
    setWindowTitle(tr("Permissions"));
    m_table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
}

PermissionsDock::~PermissionsDock()
{
}

void PermissionsDock::fillInfo()
{
#define ADD_ROW(title, function) \
do { \
    QListWidgetItem *item = new QListWidgetItem(); \
    item->setFlags(item->flags() & ~Qt::ItemIsEnabled); \
    item->setText(title); \
    item->setCheckState(document()->function() ? Qt::Checked : Qt::Unchecked); \
    m_table->addItem(item); \
} while (0)
    ADD_ROW("Print", okToPrint);
    ADD_ROW("PrintHiRes", okToPrintHighRes);
    ADD_ROW("Change", okToChange);
    ADD_ROW("Copy", okToCopy);
    ADD_ROW("Add Notes", okToAddNotes);
    ADD_ROW("Fill Forms", okToFillForm);
    ADD_ROW("Create Forms", okToCreateFormFields);
    ADD_ROW("Extract for accessibility", okToExtractForAccessibility);
    ADD_ROW("Assemble", okToAssemble);
#undef ADD_ROW
}

void PermissionsDock::documentClosed()
{
    m_table->clear();
    AbstractInfoDock::documentClosed();
}
