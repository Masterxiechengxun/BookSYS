#ifndef CONNSERVICE_H
#define CONNSERVICE_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QByteArray>
#include <QHostAddress>
#include <QFile>
#include <QCoreApplication>
#include <QTimer>
#include <QStringList>
#include <QMessageBox>

class connService :public  QObject
{
    Q_OBJECT
public:
    static connService *getService()
    {
        static connService instance;
        return &instance;
    } 
    void dataWrite(const QString &str);
//    void disConnectService();
    QByteArray getBuf();
    bool isConnected();
    void setPWD(QString p);
signals:
    void onAcceptFile(QString);

private:
    connService(QObject *parent = 0);
    ~connService();

    QTcpSocket *socket;
    QHostAddress hostAddr;
    qint64 tport;
    QByteArray buf;
    QTimer *iTimer;
    volatile bool isFile;
    qint64 sizeTotal;
    qint64 sizeGet;
    qint64 fileNameSize;
    QFile *file;
    QString fileName;
    QString pwd;
private slots:
    //void slotDisConnection();
    void dataRead();
    void onTimerOut();
};
#endif // CONNSERVICE_H
