#include "registerdialog.h"
#include "ui_registerdialog.h"

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_pushBtn_clicked()
{
    if (ui->nameLine->text().isEmpty() || ui->pswLine->text().isEmpty()) {
        QMessageBox::about(this, tr("警告"), tr("必须输入用户名和密码!"));
        ui->nameLine->clear();
        ui->pswLine->clear();
        ui->repswLine->clear();
        ui->nameLine->setFocus();
    } else if (ui->repswLine->text().trimmed() != ui->pswLine->text().trimmed()) {
        QMessageBox::about(this, tr("警告"), tr("两次输入的密码必须相同!"));
        ui->nameLine->clear();
        ui->pswLine->clear();
        ui->repswLine->clear();
        ui->nameLine->setFocus();
    } else { 
        QString name,password,mail,phone;
        name = ui->nameLine->text().trimmed();
        password = ui->pswLine->text().trimmed();
        if(!ui->mailLine->text().isEmpty())
        {
            mail = ui->mailLine->text().trimmed();
        } else {
            mail = '#';
        }
        if(!ui->phoneLine->text().isEmpty())
        {
            phone = ui->phoneLine->text().trimmed();
        } else {
            phone = '#';
        }
        connService *conn = connService::getService();
        QString info = QString("register %1 %2 %3 %4").arg(name).arg(password).arg(mail).arg(phone);
        if(conn->isConnected())
        {
            conn->dataWrite(info);
            QByteArray result;
            result = conn->getBuf();
            if(static_cast<QString>(result) =="true")
            {
                accept();
                QMessageBox::about(this, tr("恭喜"), tr("账户注册成功!"));
            } else {
                QMessageBox::about(this, tr("提醒"), tr("账户注册失败!"));
            }
        }
    }
}
