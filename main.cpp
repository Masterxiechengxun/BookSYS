#include "mainwindow.h"
#include "logindialog.h"
#include "registerdialog.h"
#include "connservice.h"

#include <QApplication>
#include <QDialog>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    const QStringList args = QCoreApplication::arguments();
    //connService::getService();
    MainWindow *viewer = new MainWindow();
    LoginDialog l(0, viewer);
    l.setWindowTitle("MyPDFSYS");
    l.setWindowIcon(QIcon(":/image/bdragon"));
    if(l.exec() == QDialog::Accepted)
    {
        viewer->show();
        if (args.count() > 1)
        {
            viewer->loadDocument(args.at(1));
        }
        return app.exec();
    }
    else
        return -1;
}
