#include "navigationtoolbar.h"

#include "poppler-qt5.h"

#include <QtWidgets/QAction>
#include <QtWidgets/QComboBox>

NavigationToolBar::NavigationToolBar(QWidget *parent)
    : QToolBar(parent)
{
    m_firstAct = addAction(tr("First"), this, SLOT(slotGoFirst()));
    m_prevAct = addAction(tr("Previous"), this, SLOT(slotGoPrev()));
    m_pageCombo = new QComboBox(this);
    connect(m_pageCombo, SIGNAL(activated(int)), this, SLOT(slotComboActivated(int)));
    addWidget(m_pageCombo);
    m_nextAct = addAction(tr("Next"), this, SLOT(slotGoNext()));
    m_lastAct = addAction(tr("Last"), this, SLOT(slotGoLast()));

    addSeparator();

    m_zoomCombo = new QComboBox(this);
    m_zoomCombo->setEditable(true);
    m_zoomCombo->addItem(tr("10%"));
    m_zoomCombo->addItem(tr("25%"));
    m_zoomCombo->addItem(tr("33%"));
    m_zoomCombo->addItem(tr("50%"));
    m_zoomCombo->addItem(tr("66%"));
    m_zoomCombo->addItem(tr("75%"));
    m_zoomCombo->addItem(tr("100%"));
    m_zoomCombo->addItem(tr("125%"));
    m_zoomCombo->addItem(tr("150%"));
    m_zoomCombo->addItem(tr("200%"));
    m_zoomCombo->addItem(tr("300%"));
    m_zoomCombo->addItem(tr("400%"));
    m_zoomCombo->setCurrentIndex(6); // "100%"
    connect(m_zoomCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotZoomComboChanged(QString)));
    addWidget(m_zoomCombo);

    m_rotationCombo = new QComboBox(this);
    // NOTE: \302\260 = degree symbol
    m_rotationCombo->addItem(trUtf8("0\302\260"));
    m_rotationCombo->addItem(trUtf8("90\302\260"));
    m_rotationCombo->addItem(trUtf8("180\302\260"));
    m_rotationCombo->addItem(trUtf8("270\302\260"));
    connect(m_rotationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(slotRotationComboChanged(int)));
    addWidget(m_rotationCombo);

    documentClosed();
}

NavigationToolBar::~NavigationToolBar()
{
}

void NavigationToolBar::documentLoaded()
{
    const int pageCount = document()->numPages();
    for (int i = 0; i < pageCount; ++i) {
        m_pageCombo->addItem(QString::number(i + 1));
    }
    m_pageCombo->setEnabled(true);
}

void NavigationToolBar::documentClosed()
{
    m_firstAct->setEnabled(false);
    m_prevAct->setEnabled(false);
    m_nextAct->setEnabled(false);
    m_lastAct->setEnabled(false);
    m_pageCombo->clear();
    m_pageCombo->setEnabled(false);
}

void NavigationToolBar::pageChanged(int page)
{
    const int pageCount = document()->numPages();
    m_firstAct->setEnabled(page > 0);
    m_prevAct->setEnabled(page > 0);
    m_nextAct->setEnabled(page < (pageCount - 1));
    m_lastAct->setEnabled(page < (pageCount - 1));
    m_pageCombo->setCurrentIndex(page);
}

void NavigationToolBar::slotGoFirst()
{
    setPage(0);
}

void NavigationToolBar::slotGoPrev()
{
    setPage(page() - 1);
}

void NavigationToolBar::slotGoNext()
{
    setPage(page() + 1);
}

void NavigationToolBar::slotGoLast()
{
    setPage(document()->numPages() - 1);
}

void NavigationToolBar::slotComboActivated(int index)
{
    setPage(index);
}

void NavigationToolBar::slotZoomComboChanged(const QString &_text)
{
    QString text = _text;
    text.remove(QLatin1Char('%'));
    bool ok = false;
    int value = text.toInt(&ok);
    if (ok && value >= 10) {
        emit zoomChanged(qreal(value) / 100);
    }
}

void NavigationToolBar::slotRotationComboChanged(int idx)
{
    emit rotationChanged(idx * 90);
}
