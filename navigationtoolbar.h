#ifndef NAVIGATIONTOOLBAR_H
#define NAVIGATIONTOOLBAR_H

#include <QtWidgets/QToolBar>

#include "documentobserver.h"

class QAction;
class QComboBox;

class NavigationToolBar : public QToolBar, public DocumentObserver
{
    Q_OBJECT

public:
    NavigationToolBar(QWidget *parent = 0);
    ~NavigationToolBar();

    void documentLoaded() override;
    void documentClosed() override;
    void pageChanged(int page) override;

Q_SIGNALS:
    void zoomChanged(qreal value);
    void rotationChanged(int rotation);

private Q_SLOTS:
    void slotGoFirst();
    void slotGoPrev();
    void slotGoNext();
    void slotGoLast();
    void slotComboActivated(int index);
    void slotZoomComboChanged(const QString &text);
    void slotRotationComboChanged(int idx);

private:
    QAction *m_firstAct;
    QAction *m_prevAct;
    QComboBox *m_pageCombo;
    QAction *m_nextAct;
    QAction *m_lastAct;
    QComboBox *m_zoomCombo;
    QComboBox *m_rotationCombo;
};

#endif
