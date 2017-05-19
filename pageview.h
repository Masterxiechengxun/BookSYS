#ifndef PAGEVIEW_H
#define PAGEVIEW_H

#include <QtWidgets/QScrollArea>

#include "documentobserver.h"

class QLabel;

class PageView : public QScrollArea, public DocumentObserver
{
    Q_OBJECT

public:
    PageView(QWidget *parent = 0);
    ~PageView();

    void documentLoaded() override;
    void documentClosed() override;
    void pageChanged(int page) override;

private Q_SLOTS:
    void slotZoomChanged(qreal value);
    void slotRotationChanged(int value);

private:
    QLabel *m_imageLabel;
    qreal m_zoom;
    int m_rotation;
    int m_dpiX;
    int m_dpiY;
};

#endif
