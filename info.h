#ifndef INFO_H
#define INFO_H

#include "abstractinfodock.h"

class QTableWidget;

class InfoDock : public AbstractInfoDock
{
    Q_OBJECT

public:
    InfoDock(QWidget *parent = 0);
    ~InfoDock();

    void documentClosed() override;

protected:
    void fillInfo() override;

private:
    QTableWidget *m_table;
};

#endif
