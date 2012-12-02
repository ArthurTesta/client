#ifndef PROTOCOL_H
#define PROTOCOL_Hs
#include <QString>
#include <QtNetwork>

class Exception;
QString & getQStringFromSock(QTcpSocket * t);
void readChar(char * dest, int length,QTcpSocket * t);
void readInt(int * dest, QTcpSocket * t);
QByteArray & getDataFromSock(QTcpSocket * t) throw (Exception);
void writeInt(int * source, QTcpSocket * t);
void writeQString(QString & source,QTcpSocket * t);

#endif // PROTOCOL_H
