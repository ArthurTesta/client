#ifndef CORE_H
#define CORE_H

#include <QFileInfo>
#include <QList>
#include <QtNetwork>
#include "exception.h"
#include "protocol.h"
class MainWindow;
class Core : QObject
{
    Q_OBJECT

    QList<QFileInfo *> * listFiles;
    int uploadPort,streamPort;
    QTcpSocket uploadSocket, streamSocket;
    QString serverIP;

public:
    friend class MainWindow;
    friend QString & getQStringFromSock(QTcpSocket * t);
    friend void readChar(char * dest, int length,QTcpSocket * t);
    friend void readInt(int * dest, QTcpSocket * t);
    friend QString & convertCharStoQString(char * source);
    friend QByteArray & getDataFromSock(QTcpSocket * t) throw (Exception);
    friend void writeInt(int * source, QTcpSocket * t);
    friend void writeQString(QString & source, QTcpSocket * t);

    Core();
    void setFile(QFileInfo *);
    void eraseList();
    ~Core();

private :
    bool sendFile(QString fileName,QString fileDescription);
    bool isSocketConnected(bool type);
    bool initConnection(bool type);

private slots:
    void createAction();
};

#endif // CORE_H
