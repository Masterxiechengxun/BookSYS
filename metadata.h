#ifndef METADATA_H
#define METADATA_H

#include "abstractinfodock.h"

class QTextEdit;

class MetadataDock : public AbstractInfoDock
{
    Q_OBJECT

public:
    MetadataDock(QWidget *parent = 0);
    ~MetadataDock();

    void documentClosed() override;

protected:
    void fillInfo() override;

private:
    QTextEdit *m_edit;
};

#endif
