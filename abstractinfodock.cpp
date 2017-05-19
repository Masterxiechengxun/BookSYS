#include "fonts.h"

AbstractInfoDock::AbstractInfoDock(QWidget *parent)
    : QDockWidget(parent), m_filled(false)
{
    connect(this, SIGNAL(visibilityChanged(bool)), SLOT(slotVisibilityChanged(bool)));
}

AbstractInfoDock::~AbstractInfoDock()
{
}

void AbstractInfoDock::documentLoaded()
{
    if (!isHidden()) {
        fillInfo();
        m_filled = true;
    }
}

void AbstractInfoDock::documentClosed()
{
    m_filled = false;
}

void AbstractInfoDock::pageChanged(int page)
{
    Q_UNUSED(page)
}

void AbstractInfoDock::slotVisibilityChanged(bool visible)
{
    if (visible && document() && !m_filled) {
        fillInfo();
        m_filled = true;
    }
}
