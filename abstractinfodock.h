#ifndef ABSTRACTINFODOCK_H
#define ABSTRACTINFODOCK_H

#include <QtWidgets/QDockWidget>

#include "documentobserver.h"

class AbstractInfoDock : public QDockWidget, public DocumentObserver
{
    Q_OBJECT

public:
    AbstractInfoDock(QWidget *parent = 0);
    ~AbstractInfoDock();

    void documentLoaded() override;
    void documentClosed() override;
    void pageChanged(int page) override;

protected:
    virtual void fillInfo() = 0;

private Q_SLOTS:
    void slotVisibilityChanged(bool visible);

private:
    bool m_filled;
};

#endif
