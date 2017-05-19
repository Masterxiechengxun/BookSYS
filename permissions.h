#ifndef PERMISSIONS_H
#define PERMISSIONS_H

#include "abstractinfodock.h"

class QListWidget;

class PermissionsDock : public AbstractInfoDock
{
    Q_OBJECT

public:
    PermissionsDock(QWidget *parent = 0);
    ~PermissionsDock();

    void documentClosed() override;

protected:
    void fillInfo() override;

private:
    QListWidget *m_table;
};

#endif
