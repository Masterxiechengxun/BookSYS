#ifndef OPTCONTENT_H
#define OPTCONTENT_H

#include "abstractinfodock.h"

class QTreeView;

class OptContentDock : public AbstractInfoDock
{
    Q_OBJECT

public:
    OptContentDock(QWidget *parent = 0);
    ~OptContentDock();

    void documentLoaded() override;
    void documentClosed() override;

protected:
    void fillInfo() override;

private Q_SLOTS:
    void reloadImage();

private:
    QTreeView *m_view;
};

#endif
