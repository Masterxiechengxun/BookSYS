#include "MainWindow.h"

#include "embeddedfiles.h"
#include "fonts.h"
#include "info.h"
#include "metadata.h"
#include "navigationtoolbar.h"
#include "optcontent.h"
#include "pageview.h"
#include "permissions.h"
#include "thumbnails.h"

#include "poppler-qt5.h"

#include <QtCore/QDir>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QStatusBar>
#include <QHBoxLayout>
#include <QFile>


MainWindow::MainWindow():QMainWindow(), m_currentPage(0), m_doc(0)
{
    resize(1000,650);
    setWindowTitle("PDFSYS");
    setWindowIcon(QIcon(":/image/bdragon"));
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    m_fileOpenAct = fileMenu->addAction(tr("&Open"), this, SLOT(slotOpenFile()));
    m_fileOpenAct->setShortcut(Qt::CTRL + Qt::Key_O);
    fileMenu->addSeparator();
    m_fileSaveCopyAct = fileMenu->addAction(tr("&Save a Copy..."), this, SLOT(slotSaveCopy()));
    m_fileSaveCopyAct->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_S);
    m_fileSaveCopyAct->setEnabled(false);
    fileMenu->addSeparator();
    QAction *act = fileMenu->addAction(tr("&Quit"), qApp, SLOT(closeAllWindows()));
    act->setShortcut(Qt::CTRL + Qt::Key_Q);

    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));

    QMenu *settingsMenu = menuBar()->addMenu(tr("&Settings"));
    m_settingsTextAAAct = settingsMenu->addAction(tr("Text Antialias"));
    m_settingsTextAAAct->setCheckable(true);
    connect(m_settingsTextAAAct, SIGNAL(toggled(bool)), this, SLOT(slotToggleTextAA(bool)));
    m_settingsGfxAAAct = settingsMenu->addAction(tr("Graphics Antialias"));
    m_settingsGfxAAAct->setCheckable(true);
    connect(m_settingsGfxAAAct, SIGNAL(toggled(bool)), this, SLOT(slotToggleGfxAA(bool)));
    QMenu *settingsRenderMenu = settingsMenu->addMenu(tr("Render Backend"));
    m_settingsRenderBackendGrp = new QActionGroup(settingsRenderMenu);
    m_settingsRenderBackendGrp->setExclusive(true);
    act = settingsRenderMenu->addAction(tr("Splash"));
    act->setCheckable(true);
    act->setChecked(true);
    act->setData(qVariantFromValue(0));
    m_settingsRenderBackendGrp->addAction(act);
    act = settingsRenderMenu->addAction(tr("Arthur"));
    act->setCheckable(true);
    act->setData(qVariantFromValue(1));
    m_settingsRenderBackendGrp->addAction(act);
    connect(m_settingsRenderBackendGrp, SIGNAL(triggered(QAction*)),
            this, SLOT(slotRenderBackend(QAction*)));

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    act = helpMenu->addAction(tr("&About &Viewer"), this, SLOT(slotAbout()));
    act = helpMenu->addAction(tr("&About &You"), this, SLOT(slotAboutYou()));
    act = helpMenu->addAction(tr("About &Qt"), this, SLOT(slotAboutQt()));
    NavigationToolBar *navbar = new NavigationToolBar(this);
    addToolBar(navbar);
    m_observers.append(navbar);

    InfoDock *infoDock = new InfoDock(this);
    addDockWidget(Qt::LeftDockWidgetArea, infoDock);
    infoDock->hide();
    viewMenu->addAction(infoDock->toggleViewAction());
    m_observers.append(infoDock);

    FontsDock *fontsDock = new FontsDock(this);
    addDockWidget(Qt::LeftDockWidgetArea, fontsDock);
    fontsDock->hide();
    viewMenu->addAction(fontsDock->toggleViewAction());
    m_observers.append(fontsDock);

    PermissionsDock *permissionsDock = new PermissionsDock(this);
    addDockWidget(Qt::LeftDockWidgetArea, permissionsDock);
    permissionsDock->hide();
    viewMenu->addAction(permissionsDock->toggleViewAction());
    m_observers.append(permissionsDock);

    ThumbnailsDock *thumbnailsDock = new ThumbnailsDock(this);
    addDockWidget(Qt::LeftDockWidgetArea, thumbnailsDock);
    thumbnailsDock->hide();
    viewMenu->addAction(thumbnailsDock->toggleViewAction());
    m_observers.append(thumbnailsDock);

    EmbeddedFilesDock *embfilesDock = new EmbeddedFilesDock(this);
    addDockWidget(Qt::BottomDockWidgetArea, embfilesDock);
    embfilesDock->hide();
    viewMenu->addAction(embfilesDock->toggleViewAction());
    m_observers.append(embfilesDock);

    MetadataDock *metadataDock = new MetadataDock(this);
    addDockWidget(Qt::BottomDockWidgetArea, metadataDock);
    metadataDock->hide();
    viewMenu->addAction(metadataDock->toggleViewAction());
    m_observers.append(metadataDock);

    OptContentDock *optContentDock = new OptContentDock(this);
    addDockWidget(Qt::LeftDockWidgetArea, optContentDock);
    optContentDock->hide();
    viewMenu->addAction(optContentDock->toggleViewAction());
    m_observers.append(optContentDock);

    QWidget *taskArea = new QWidget();
    setCentralWidget(taskArea);
    PageView *view = new PageView(this);
    m_observers.append(view);
    list = BookList::getBookList(this);
    connService *conn = connService::getService();
    connect(conn, SIGNAL(onAcceptFile(QString)), this, SLOT(slotSelectBook(QString)));
    connect(list, SIGNAL(onSelectBook(QString)), this, SLOT(slotSelectBook(QString)));
    connect(list, SIGNAL(onReceiveBook()), this, SLOT(slotReceiveBook()));

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(list);
    hlayout->addWidget(view);
    hlayout->setSpacing(0);
    hlayout->setStretchFactor(list, 1);
    hlayout->setStretchFactor(view, 4);
    taskArea->setLayout(hlayout);
    Q_FOREACH(DocumentObserver *obs, m_observers) {
        obs->m_viewer = this;
    }

    connect(navbar, SIGNAL(zoomChanged(qreal)), view, SLOT(slotZoomChanged(qreal)));
    connect(navbar, SIGNAL(rotationChanged(int)), view, SLOT(slotRotationChanged(int)));

    // activate AA by default
    m_settingsTextAAAct->setChecked(true);
    m_settingsGfxAAAct->setChecked(true);

    statusBar()->setFixedHeight(18);
    statusBar()->setMinimumWidth(this->width());
    statusBar()->setStyleSheet(QString("QStatusBar::item{border: 0px}"));
    statusInfo = new QLabel;
    statusInfo->setFixedHeight(18);
    progressBar = new QProgressBar(this);
    progressBar->setFixedHeight(15);
    progressBar->setMinimumWidth(this->width());
    progressBar->setAlignment(Qt::AlignLeft);
    progressBar->setRange(0, 0);
    progressBar->setValue(0);
    progressBar->setTextVisible(false);
    statusBar()->addWidget(progressBar);
    statusBar()->addWidget(statusInfo);
    statusInfo->hide();
    progressBar->hide();
}

MainWindow::~MainWindow()
{
    closeDocument();
}

QSize MainWindow::sizeHint() const
{
    return QSize(500, 600);
}

void MainWindow::loadDocument(const QString &file)
{
    Poppler::Document *newdoc = Poppler::Document::load(file);
    if (!newdoc) {
        QMessageBox msgbox(QMessageBox::Critical, tr("Open Error"), tr("Cannot open:\n") + file,
                           QMessageBox::Ok, this);
        msgbox.exec();
        return;
    }

    while (newdoc->isLocked()) {
        bool ok = true;
        QString password = QInputDialog::getText(this, tr("Document Password"),
                                                 tr("Please insert the password of the document:"),
                                                 QLineEdit::Password, QString(), &ok);
        if (!ok) {
            delete newdoc;
            return;
        }
        newdoc->unlock(password.toLatin1(), password.toLatin1());
    }

    closeDocument();

    m_doc = newdoc;
    m_doc->setRenderHint(Poppler::Document::TextAntialiasing, m_settingsTextAAAct->isChecked());
    m_doc->setRenderHint(Poppler::Document::Antialiasing, m_settingsGfxAAAct->isChecked());
    m_doc->setRenderBackend((Poppler::Document::RenderBackend)m_settingsRenderBackendGrp->checkedAction()->data().toInt());

    Q_FOREACH(DocumentObserver *obs, m_observers) {
        obs->documentLoaded();
        obs->pageChanged(0);
    }

    m_fileSaveCopyAct->setEnabled(true);
}

void MainWindow::closeDocument()
{
    if (!m_doc) {
        return;
    }

    Q_FOREACH(DocumentObserver *obs, m_observers) {
        obs->documentClosed();
    }
    m_currentPage = 0;
    delete m_doc;
    m_doc = 0;
    m_fileSaveCopyAct->setEnabled(false);
}

void MainWindow::slotOpenFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open PDF Document"), QDir::homePath(), tr("PDF Documents (*.pdf)"));
    if (fileName.isEmpty()) {
        return;
    }
    closeDocument();
    loadDocument(fileName);
}

void MainWindow::slotSaveCopy()
{
    if (!m_doc) {
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Copy"), QDir::homePath(), tr("PDF Documents (*.pdf)"));
    if (fileName.isEmpty()) {
        return;
    }

    Poppler::PDFConverter *converter = m_doc->pdfConverter();
    converter->setOutputFileName(fileName);
    converter->setPDFOptions(converter->pdfOptions() & ~Poppler::PDFConverter::WithChanges);
    if (!converter->convert()) {
        QMessageBox msgbox(QMessageBox::Critical, tr("Save Error"), tr("Cannot export to:\n%1").arg(fileName),
                           QMessageBox::Ok, this);
    }
    delete converter;
}

void MainWindow::slotAbout()
{
    const QString text("This is an encrypted electronic book distribution reading system.");
    QMessageBox::about(this, QString::fromLatin1("About PDF Viewer"), text);
}

void MainWindow::slotAboutQt()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::slotAboutYou()
{
    const QString text = tr("name: %1").arg(usr_name);
    QMessageBox::about(this, QString::fromLatin1("About your information"), text);
}

void MainWindow::slotToggleTextAA(bool value)
{
    if (!m_doc) {
        return;
    }

    m_doc->setRenderHint(Poppler::Document::TextAntialiasing, value);

    Q_FOREACH(DocumentObserver *obs, m_observers) {
        obs->pageChanged(m_currentPage);
    }
}

void MainWindow::slotToggleGfxAA(bool value)
{
    if (!m_doc) {
        return;
    }

    m_doc->setRenderHint(Poppler::Document::Antialiasing, value);

    Q_FOREACH(DocumentObserver *obs, m_observers) {
        obs->pageChanged(m_currentPage);
    }
}

void MainWindow::slotRenderBackend(QAction *act)
{
    if (!m_doc || !act) {
        return;
    }

    m_doc->setRenderBackend((Poppler::Document::RenderBackend)act->data().toInt());

    Q_FOREACH(DocumentObserver *obs, m_observers) {
        obs->pageChanged(m_currentPage);
    }
}

void MainWindow::setPage(int page)
{
    Q_FOREACH(DocumentObserver *obs, m_observers) {
        obs->pageChanged(page);
    }
    m_currentPage = page;
}

int MainWindow::page() const
{
    return m_currentPage;
}

void MainWindow::wheelEvent(QWheelEvent *e)
{
    static quint64 repeatCall = 0;
    ++repeatCall;
    if(repeatCall % 2 == 1)
    {
        if (!m_doc) {
            return;
        }
        if((e->delta() > 0) && (page() > 0))
            setPage(page() - 1);
        else if((e->delta() < 0) && (page() < (m_doc->numPages() - 1)))
            setPage(page() + 1);
    }
}

void MainWindow::slotGetUserINFO(QString name, QString psw)
{
    usr_name = name;
    usr_psw  = psw;
    if(usr_name == "Guest")
    {
        list->hide();
        list->setEnabled(false);
        statusBar()->hide();
    }
}

void MainWindow::slotSelectBook(QString fileName)
{
    if(fileName == "")
        return;
    statusInfo->setText("decoding......");
    startThread(fileName, usr_psw);
}

void MainWindow::startThread(QString file, QString pwd)
{
    myThread = new workThread(this, file, pwd);
    connect(myThread, SIGNAL(onCryptoFinish(QString)), this, SLOT(slotCryptoFinish(QString)));
    connect(myThread, SIGNAL(finished()), myThread, SLOT(deleteLater()));
    myThread->start();
}
void MainWindow::slotReceiveBook()
{
    statusInfo->setText("Receive......");
    statusInfo->show();
    progressBar->show();
}

void MainWindow::slotCryptoFinish(QString fileName)
{
    qDebug() << "解密结束信号结束到。";
    QString curFileName = fileName.right(fileName.size() - fileName.lastIndexOf('\\') - 1);
    QString cryptoFileName = fileName.left(fileName.lastIndexOf('\\') + 1);
    cryptoFileName = cryptoFileName + "EncrypteBooks\\" + curFileName;
    closeDocument();
    statusInfo->hide();
    progressBar->hide();
    loadDocument(cryptoFileName);
    //让目录区域再次接受鼠标点击事件
    list->setEnabled(true);
}
