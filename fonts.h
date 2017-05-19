#ifndef FONTS_H
#define FONTS_H

#include "abstractinfodock.h"

class QTableWidget;

class FontsDock : public AbstractInfoDock
{
    Q_OBJECT

public:
    FontsDock(QWidget *parent = 0);
    ~FontsDock();

    void documentClosed() override;

protected:
    void fillInfo() override;

private:
    QTableWidget *m_table;
};

#endif
