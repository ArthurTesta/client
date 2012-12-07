#ifndef TRANSFERMESSAGE_H
#define TRANSFERMESSAGE_H
#include <QString>
#include <string>
#include <stdlib.h>
class TransferMessage : QString
{
private :
    int code;
    QString msg;
public:
    TransferMessage(QString msg, int code);
    TransferMessage(std::string msg, int code);
    TransferMessage(const char * msg,int code);
    QString getMessage();
    int getCode();
    void setMessage(QString msg);
    void setMessage(std::string msg);
    void setCode(int newCode);
};

#endif // TRANSFERMESSAGE_H
