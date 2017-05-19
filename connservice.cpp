#include "connservice.h"

//void connService::disConnectService()
//{
//    if(socket)
//    {
//        socket->disconnectFromHost();
//        socket->close();
//        socket->abort();
//    }
//}
connService::connService(QObject *parent):
    QObject(parent)
{
    tport = 6666;
    hostAddr = QHostAddress::LocalHost;
    sizeTotal = 0;
    sizeGet = 0;
    fileNameSize = 0;
    fileName.clear();
    file = NULL;
    isFile = false;
    buf.resize(0);
    socket = new QTcpSocket;
    socket->abort();
    socket->connectToHost(("127.0.0.1"), 6666);
    connect(socket, SIGNAL(connected()), this, SLOT(slotConnetion()));
    //connect(socket, SIGNAL(disconnected()), this, SLOT(slotDisConnection()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(dataRead()));
}
connService::~connService()
{
    if(socket != NULL)
    {
        socket->disconnectFromHost();
        socket->close();
        socket->abort();
        delete socket;
    }
}
void connService::slotConnetion()
{
}
//void connService::slotDisConnection()
//{
//    socket->abort();
//    socket->connectToHost(("127.0.0.1"), 6666);
//    connect(socket, SIGNAL(connected()), this, SLOT(connService::slotConnetion()));
//    connect(socket, SIGNAL(disconnected()), this, SLOT(connService::slotDisConnection()));
//    connect(socket, SIGNAL(readyRead()), this, SLOT(connService::dataRead()));
//}
void connService::dataWrite(const QString &str)
{
    socket->write(str.toUtf8(),str.length() + 1);
    qDebug() << "has write:" << str;
}
void connService::dataRead()
{
    if(!isFile)
    {
        if(socket->bytesAvailable() > 0)
        {
            buf.resize(0);
            buf = socket->readAll();
            qDebug() << buf.data();
        }
        QStringList list = (static_cast<QString>(buf)).split('#');
        if(list.at(0) == "file")
        {
            isFile = true;
            sizeTotal = (list.at(1)).toInt();
            fileName = list.at(2);
//            QStringList l = fileName.split('.');
//            fileName = l.at(0) + ".mypdf";
            fileName ="C:\\pdfsys\\localFiles\\" + fileName;
            file = new QFile(fileName);
            qDebug() << "has get the file head : " << fileName;
            if(file->exists())
                file->remove();
            if(!file->open(QFile::WriteOnly))
            {
                qDebug() << "无法写文件";
                buf.resize(0);
                return;
            }
        } else qDebug() << "this is not a file body";
    } else {      
        //接受的数据小于总数据，则写入文件
        qDebug() << "has begin to get file body" << fileName;
        if(sizeGet < sizeTotal)
        {
            sizeGet += (socket->bytesAvailable());
            buf.resize(0);
            buf = socket->readAll();
            file->write(buf);
            buf.resize(0);
        }
        //数据接受完毕
        if(sizeGet == sizeTotal)
        {
            file->close();
            file = NULL;
            sizeGet = 0;
            sizeTotal = 0;
            fileNameSize = 0;
            qDebug() << QString("接受文件 %1完毕.");
            emit onAcceptFile(fileName);
            fileName.clear();
            isFile = false;
            buf.resize(0);
        }
    }
}
QByteArray  connService::getBuf()
{
    //阻塞等待socket获取到服务器的数据
    socket->waitForReadyRead();
    return buf;
}
bool connService::isConnected()
{
    return (socket->state() == QTcpSocket::ConnectedState);
}
