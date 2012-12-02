#include "core.h"
#include <QDebug>

Core::Core():streamPort(4001),uploadPort(4000),serverIP("127.0.0.1")
{
    listFiles = new QList<QFileInfo *>();
}

void Core::setFile(QFileInfo *f){
    if(f){
        listFiles->insert(listFiles->begin(), f);
    }
    for(int i=0; i<listFiles->size(); i++){
        qDebug() << listFiles->at(i)->fileName();
    }
    qDebug(); //LF
    // traitement possible du fichier
}

void Core::eraseList(){
    for(int i=0; i<listFiles->size(); i++){
        delete listFiles->at(i); // utiliser erase ?
    }
    listFiles->erase(listFiles->begin(), listFiles->end());
}

Core::~Core(){
    delete listFiles;
}

void Core::createAction(){

}

bool Core::sendFile(QString fileName,QString fileDescription){
    if(initConnection(0)){
        QFile * myFile = new QFile(fileName);
        int fileSize = myFile->size();
        myFile->open(QIODevice::ReadOnly);
        writeQString(fileName,&uploadSocket);
        writeQString(fileDescription,&uploadSocket);
        writeInt(&fileSize,&uploadSocket);
        uploadSocket.write(myFile->readAll());
        uploadSocket.flush();
        uploadSocket.waitForReadyRead(10000);
        QString result = getQStringFromSock(&uploadSocket);
        if(result.toStdString() == "Everything ok"){
            qDebug() << "[CLI] - OK";
        }
        else {
            qDebug() << "[CLI] - ERROR";
        }
    }
}
/*
  type==0 si upload
 */
bool Core::isSocketConnected(bool type){
    if(type){
        return uploadSocket.waitForConnected(0);
    }
    return streamSocket.waitForConnected(0);
}

bool Core::initConnection(bool type){
    if (type){
        uploadSocket.connectToHost(serverIP,uploadPort);
        return uploadSocket.waitForConnected(5000);
    }
    streamSocket.connectToHost(serverIP,streamPort);
    return streamSocket.waitForConnected(5000);
}
