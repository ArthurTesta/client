#include "transfermessage.h"

TransferMessage::TransferMessage(QString msg, int code):msg(msg),code(code){

}

TransferMessage::TransferMessage(std::string msg, int code):msg(QString::fromStdString(msg)),code(code){

}

TransferMessage::TransferMessage(const char * msg,int code):msg(msg),code(code){

}

QString TransferMessage::getMessage(){
    return msg;
}

int TransferMessage::getCode(){
    return code;
}

void TransferMessage::setMessage(QString newMsg){
    msg = newMsg;
}

void TransferMessage::setCode(int newCode){
    code=newCode;
}

void TransferMessage::setMessage(std::string newMsg){
    msg = QString::fromStdString(newMsg);
}


