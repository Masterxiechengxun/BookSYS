#ifndef CONNSERVICE_H
#define CONNSERVICE_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QByteArray>
#include <QHostAddress>
#include <QFile>
#include <QCoreApplication>
//#include <QTime>
#include <QStringList>

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
    //void disConnectService();
    QByteArray getBuf();
    bool isConnected();
signals:
    void onAcceptFile(QString);
private:
    connService(QObject *parent = 0);
    ~connService();
    QTcpSocket *socket;
    QHostAddress hostAddr;
    qint64 tport;
    QByteArray buf;
   // QTime time;
    bool isFile;
    qint64 sizeTotal;
    qint64 sizeGet;
    qint64 fileNameSize;
    QFile *file;
    QString fileName;
private slots:
    void slotConnetion();
    //void slotDisConnection();
    void dataRead();
};
#endif // CONNSERVICE_H
