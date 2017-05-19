#include "optcontent.h"

#include "poppler-qt5.h"

#include <QtWidgets/QTreeView>

OptContentDock::OptContentDock(QWidget *parent)
    : AbstractInfoDock(parent)
{
    m_view = new QTreeView(this);
    setWidget(m_view);
    setWindowTitle(tr("Optional content"));
    m_view->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
}

OptContentDock::~OptContentDock()
{
}


void OptContentDock::documentLoaded()
{
    AbstractInfoDock::documentLoaded();
    if ( document()->pageMode() == Poppler::Document::UseOC ) {
        show();
    }  
}

void OptContentDock::fillInfo()
{
    if (!document()->hasOptionalContent()) {
        return;
    }

    m_view->setModel(document()->optionalContentModel());
    connect(m_view->model(), SIGNAL(dataChanged(QModelIndex, QModelIndex)), this, SLOT(reloadImage()));
    m_view->expandToDepth(1);
}

void OptContentDock::documentClosed()
{
    m_view->setModel(0);
    AbstractInfoDock::documentClosed();
}

void OptContentDock::reloadImage()
{
    reloadPage();
}
