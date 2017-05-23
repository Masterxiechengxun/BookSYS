#include "workthread.h"

workThread::workThread(QObject *parent, QString file, QString p):
    QThread(parent)
{
    fileName = file;
    usr_psw = p;
}
void workThread::cryptoFile()
{
    QFile *ifile = new QFile(fileName);
    //通过计算用户密码的Hash值来获得AES秘钥
    QString pwd = usr_psw;
    QByteArray key;
    QByteArray p;
    key = QCryptographicHash::hash(p.append(pwd), QCryptographicHash::Md5);
    unsigned char *aesKey = (unsigned char *)key.data();
    QString curFileName = fileName.right(fileName.size() - fileName.lastIndexOf('\\') - 1);
    QString cryptoFileName = fileName.left(fileName.lastIndexOf('\\') + 1);
    //创建解密图书用的工作目录
    QDir *workDir = new QDir(cryptoFileName + "EncrypteBooks");
    if(!workDir->exists())
        workDir->mkdir(cryptoFileName + "EncrypteBooks");

    cryptoFileName = cryptoFileName + "EncrypteBooks\\" + curFileName;
    QFile *ofile = new QFile(cryptoFileName);
    if(ofile->exists())
        ofile->remove();
    //开始AES文件解密
    AES aes(Bits128, aesKey);
    aes.FileInvCipher(ifile, ofile);
    ifile->remove();
}
