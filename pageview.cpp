#include "pageview.h"

#include "poppler-qt5.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>
#include <QtGui/QImage>
#include <QtWidgets/QLabel>
#include <QtGui/QPixmap>
#include <QScrollBar>
#include <QDebug>

PageView::PageView(QWidget *parent)
    : QScrollArea(parent)
    , m_zoom(1.0)
    , m_rotation(0)
    , m_dpiX(QApplication::desktop()->physicalDpiX())
    , m_dpiY(QApplication::desktop()->physicalDpiY())
{
    m_imageLabel = new QLabel(this);
    m_imageLabel->resize(0, 0);
    setWidget(m_imageLabel);
    setAlignment(Qt::AlignCenter); //设置QLabel对齐QScrollArea中间
    setBackgroundRole(QPalette::Dark);
}

PageView::~PageView()
{
}

void PageView::documentLoaded()
{
}

void PageView::documentClosed()
{
    m_imageLabel->clear();
    m_imageLabel->resize(0, 0);
}

void PageView::pageChanged(int page)
{
    //每次翻页滚动条自动回到顶部或者顶部
    if((this->verticalScrollBar()->sliderPosition()) == (this->verticalScrollBar()->maximum()))
    {
        this->verticalScrollBar()->setSliderPosition(this->verticalScrollBar()->minimum());
    } else if((this->verticalScrollBar()->sliderPosition()) == (this->verticalScrollBar()->minimum())) {
        this->verticalScrollBar()->setSliderPosition(this->verticalScrollBar()->maximum());
    }

    Poppler::Page *popplerPage = document()->page(page);
    const double resX = m_dpiX * m_zoom;
    const double resY = m_dpiY * m_zoom;

    Poppler::Page::Rotation rot;
    if (m_rotation == 0)
        rot = Poppler::Page::Rotate0;
    else if (m_rotation == 90)
        rot = Poppler::Page::Rotate90;
    else if (m_rotation == 180)
        rot = Poppler::Page::Rotate180;
    else // m_rotation == 270
        rot = Poppler::Page::Rotate270;

    QImage image = popplerPage->renderToImage(resX, resY, -1, -1, -1, -1, rot);
    if (!image.isNull()) {   
        m_imageLabel->resize(image.size());
        m_imageLabel->setPixmap(QPixmap::fromImage(image));
    } else {
        m_imageLabel->resize(0, 0);
        m_imageLabel->setPixmap(QPixmap());
    }
    delete popplerPage;
}

void PageView::slotZoomChanged(qreal value)
{
    m_zoom = value;
    if (!document()) {
        return;
    }
    reloadPage();
}

void PageView::slotRotationChanged(int value)
{
    m_rotation = value;
    if (!document()) {
        return;
    }
    reloadPage();
}
