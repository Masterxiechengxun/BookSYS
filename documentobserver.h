#ifndef DOCUMENTOBSERVER_H
#define DOCUMENTOBSERVER_H

class MainWindow;
namespace Poppler {
class Document;
}

class DocumentObserver
{
friend class MainWindow;

public:
    virtual ~DocumentObserver();
    virtual void documentLoaded() = 0;
    virtual void documentClosed() = 0;
    virtual void pageChanged(int page) = 0;

protected:
    DocumentObserver();
    Poppler::Document* document() const;
    void setPage(int page);
    int page() const;
    void reloadPage();

private:
    MainWindow *m_viewer;
};

#endif
