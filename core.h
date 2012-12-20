#ifndef CORE_H
#define CORE_H

#include <QFileInfo>
#include <QList>
#include <QtNetwork>
#include "exception.h"
#include "protocol.h"
class MainWindow;
class TransferMessage;
class Media;
class Core : QObject
{
    Q_OBJECT

    QList<QFileInfo *> * listFiles;
    int uploadPort,streamPort,searchPort;
    QTcpSocket uploadSocket, streamSocket,searchSocket;
    QString serverIP;

public:
    friend class MainWindow;
    friend void readCharSock(char * dest, int length,QTcpSocket * t);
    friend void readIntSock(int * dest, QTcpSocket * t);
    friend QByteArray & readDataSock(QTcpSocket * t) throw (Exception);
    friend QString & readQStringSock(QTcpSocket * t);
    friend void writeIntSock(int * source, QTcpSocket * t);
    friend void writeQStringSock(QString & source,QTcpSocket * t);

    Core();
    void setFile(QFileInfo *);
    void eraseList();
    ~Core();
    qint64 getFileToUploadSize(QString * filePath);
    QList<QFileInfo*> * getList();

private :
    void sendFile(QString * fileName,QString * fileDescription) throw (Exception);
    void receiveStream(QString * mediaName);
    bool isSocketConnected(int type);
    bool initConnection(int type);
    void sendSearchRequest(QString * fileName);
private slots:
    void createAction();
    void engageUpload(QString * fileName,QString * fileDescription);
    void engageSearch(QString * fileName);
    void engageStream(QString * fileName);
signals :
    void transferMsg(TransferMessage * result);
    void mediaAlikeList(QList <Media> *list);
};

#endif // CORE_H
