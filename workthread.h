#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QObject>
#include <QThread>
#include <QString>
#include <QByteArray>
#include <QFile>
#include <QDir>
#include <QCryptographicHash>
#include <QDebug>
#include "aes.h"

class workThread : public QThread
{
    Q_OBJECT
public:
    workThread(QObject *parent = 0, QString file = 0, QString p = 0);
protected:
    virtual void run() Q_DECL_OVERRIDE{
        cryptoFile();
        emit onCryptoFinish(fileName);
    }

signals:
    void onCryptoFinish(QString file);

private:
    QString fileName;
    QString usr_psw;
    void cryptoFile();
};

#endif // WORKTHREAD_H
