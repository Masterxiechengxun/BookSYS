#ifndef ATTACHMENTS_H
#define ATTACHMENTS_H

#include "abstractinfodock.h"

class QTableWidget;

class EmbeddedFilesDock : public AbstractInfoDock
{
    Q_OBJECT

public:
    EmbeddedFilesDock(QWidget *parent = 0);
    ~EmbeddedFilesDock();

    void documentLoaded() override;
    void documentClosed() override;

protected:
    void fillInfo() override;

private:
    QTableWidget *m_table;
};

#endif
