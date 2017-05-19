#ifndef THUMBNAILS_H
#define THUMBNAILS_H

#include "abstractinfodock.h"

class QListWidget;
class QListWidgetItem;

class ThumbnailsDock : public AbstractInfoDock
{
    Q_OBJECT

public:
    ThumbnailsDock(QWidget *parent = 0);
    ~ThumbnailsDock();

    void documentClosed() override;

protected:
    void fillInfo() override;

private Q_SLOTS:
    void slotItemActivated(QListWidgetItem *item);

private:
    QListWidget *m_list;
};

#endif
