#include "documentobserver.h"

#include "mainwindow.h"

DocumentObserver::DocumentObserver()
    : m_viewer(0)
{
}

DocumentObserver::~DocumentObserver()
{
    if(m_viewer)
        delete m_viewer;
}

Poppler::Document* DocumentObserver::document() const
{
    return m_viewer->m_doc;
}

void DocumentObserver::setPage(int page)
{
    m_viewer->setPage(page);
}

int DocumentObserver::page() const
{
    return m_viewer->page();
}

void DocumentObserver::reloadPage()
{
    m_viewer->setPage(m_viewer->page());
}
