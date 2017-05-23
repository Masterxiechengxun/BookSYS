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
    fileName.resize(0);
    file = NULL;
    isFile = false;
    buf.resize(0);
    iTimer = NULL;
    socket = new QTcpSocket;
    socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    socket->abort();
    socket->connectToHost(("127.0.0.1"), 6666);
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
void connService::dataWrite(const QString &str)
{
    socket->disconnectFromHost();
    socket->close();
    socket->abort();
    socket->connectToHost(("127.0.0.1"), 6666);
    //connect(socket, SIGNAL(disconnected()), this, SLOT(slotDisConnection()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(dataRead()));
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
        }
        QStringList list = (static_cast<QString>(buf)).split('#');
        if(list.at(0) == "file")
        {
            isFile = true;
            sizeTotal = (list.at(1)).toInt();
            fileName = list.at(2);
            fileName ="C:\\pdfsys\\localFiles\\" + fileName;
            file = new QFile(fileName);
            if(file->exists())
                file->remove();
            if(!file->open(QFile::WriteOnly))
            {
                qDebug() << "无法写文件";
                buf.resize(0);
                return;
            }
            //设置定时器，当超过8s文件还未传输，成功者重新请求文件传输
            iTimer = new QTimer(this);
            iTimer->setInterval(5000); //5s内如果没有完全接受到文件，则按照接受失败处理
            iTimer->setSingleShot(true);
            iTimer->start();
            connect(iTimer, SIGNAL(timeout()), this, SLOT(onTimerOut()));
            //connect(iTimer, SIGNAL(timeout()), this, SLOT(deleteLater()));
        } else
        {
            return;
        }
    } else {
        //接受的数据小于总数据，则写入文件
        if(sizeGet < sizeTotal)
        {
            buf.resize(0);
            sizeGet += (socket->bytesAvailable());
            buf = socket->readAll();
            file->write(buf);
            buf.resize(0);
        }
        //数据接受完毕
        if(sizeGet >= sizeTotal)
        {
            //接受文件成功，关闭定时器
            iTimer->stop();
            delete iTimer;
            iTimer = NULL;

            isFile = false;
            file->close();
            file = NULL;
            sizeGet = 0;
            sizeTotal = 0;
            fileNameSize = 0;
            qDebug() << QString("接受文件 %1完毕.");
            emit onAcceptFile(fileName);
            fileName.resize(0);
            buf.resize(0);
            socket->disconnectFromHost();
            socket->close();
            socket->abort();
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
void connService::onTimerOut()
{

    //接受文件失败，关闭定时器
    iTimer->stop();
    delete iTimer;
    iTimer = NULL;
    //重置变量,复位传输失败的部分文件的遗留信息
    socket->abort();
    isFile = false;
    file->close();
    file->remove();
    file = NULL;
    sizeGet = 0;
    sizeTotal = 0;
    fileNameSize = 0;
    QString bookTitle = fileName;
    bookTitle = bookTitle.right(bookTitle.size() - bookTitle.lastIndexOf('\\') - 1);
    fileName.resize(0);
    buf.resize(0);
    //重新发送文件传输请求
    dataWrite(tr("read #%1#%2").arg(bookTitle).arg(pwd));
}
void connService::setPWD(QString p)
{
    pwd = p;
}
