#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QWheelEvent>
#include <QCryptographicHash>
#include <QProgressBar>
#include <QTemporaryFile>
#include <QLabel>
#include <QFile>
#include <QDir>
#include <QCloseEvent>

#include "booklist.h"
#include "connservice.h"
#include "aes.h"
#include "workthread.h"

class QAction;
class QActionGroup;
class QLabel;
class DocumentObserver;
namespace Poppler {
class Document;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    friend class DocumentObserver;

public:
    MainWindow();
    ~MainWindow();

    QSize sizeHint() const override;

    void loadDocument(const QString &file);
    void closeDocument();

private Q_SLOTS:
    void slotOpenFile();
    void slotSaveCopy();
    void slotAbout();
    void slotAboutQt();
    void slotAboutYou();
    void slotToggleTextAA(bool value);
    void slotToggleGfxAA(bool value);
    void slotRenderBackend(QAction *act);
    void slotGetUserINFO(QString name, QString psw);
    void slotSelectBook(QString fileName);
    void slotReceiveBook();
    void slotCryptoFinish(QString fileName);

private:
    void setPage(int page);
    int page() const;
    void wheelEvent(QWheelEvent *e);
    void startThread(QString file, QString pwd);
    void closeEvent(QCloseEvent *event);

    int m_currentPage;

    QAction *m_fileOpenAct;
    QAction *m_fileSaveCopyAct;
    QAction *m_settingsTextAAAct;
    QAction *m_settingsGfxAAAct;
    QActionGroup *m_settingsRenderBackendGrp;

    BookList *list;
    QList<DocumentObserver *> m_observers;

    Poppler::Document *m_doc;

    QString usr_name;
    QString usr_psw;
    QProgressBar *progressBar;
    QLabel *statusInfo;
    workThread *myThread;
    QString workDir;
};

#endif // MAINWINDOW_H
