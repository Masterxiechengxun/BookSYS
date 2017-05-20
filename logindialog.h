#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QtSql/QSqlQuery>
#include "registerdialog.h"
#include "connservice.h"
#include "mainwindow.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0, MainWindow *slot = 0);
    ~LoginDialog();
signals:
    sendUsrINFO(QString name, QString psd);

private slots:
    void on_loginBtn_clicked();
    void on_registBtn_clicked();
    void on_guestBtn_clicked();
    void on_resetBtn_clicked();

private:
    Ui::LoginDialog *ui;
    QString usr;
};

#endif // LOGINDIALOG_H
