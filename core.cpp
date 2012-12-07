#include "core.h"
#include "transfermessage.h"

#include <QRegExp>
#include <QDebug>
#include <stdlib.h>


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

bool Core::sendFile(QString * completeFileName,QString * fileDescription) throw (Exception){
    try{
        QFile * myFile = new QFile(*completeFileName);
        int fileSize = myFile->size();
        myFile->open(QIODevice::ReadOnly);
        QRegExp regExp("/");
        QString fileName = completeFileName->remove(0,regExp.lastIndexIn((*completeFileName))+1);
        writeQStringSock(*completeFileName,&uploadSocket);
        writeQStringSock(*fileDescription,&uploadSocket);
        writeIntSock(&fileSize,&uploadSocket);
        uploadSocket.write(myFile->readAll());
        uploadSocket.flush();
        uploadSocket.waitForReadyRead(10000);
        int resultCode=-1;
        readIntSock(&resultCode,&uploadSocket);
        QString resultMSg = readQStringSock(&uploadSocket);
        emit upLoadResultMsg(new TransferMessage(resultMSg,resultCode));
        uploadSocket.close();
    }catch(Exception e){
        emit upLoadResultMsg(new TransferMessage(e.what(),-1));
    }
}
void Core::receiveStream(QString * mediaName){
    writeQStringSock(*mediaName,&streamSocket);
    streamSocket.flush();
    streamSocket.waitForReadyRead(5000);
    int mediaSize;
    QByteArray mediaData;
    readIntSock(&mediaSize,&streamSocket);
    mediaData+=readDataSock(&streamSocket);
    QString fileDestination("/home/");
    fileDestination.append(getenv("USER"));
    QFile * tmpWaitingForStream = new QFile(fileDestination);
    if(tmpWaitingForStream->open(QIODevice::Append)){
        tmpWaitingForStream->write(mediaData);
    }
    streamSocket.close();
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
    bool connectionState=false;
    if(isSocketConnected(type)){
        connectionState=true;
    }else if (!type){
        uploadSocket.connectToHost(serverIP,uploadPort);
        connectionState=uploadSocket.waitForConnected(5000);
    }else if(type){
        streamSocket.connectToHost(serverIP,streamPort);
        connectionState=streamSocket.waitForConnected(5000);
    }
    return connectionState;
}

qint64 Core::getFileToUploadSize(QString * filePath){
    return QFile(*filePath).size();
}
void Core::engageUpload(QString * fileName,QString * fileDescription){
    if(initConnection(0)){
        sendFile(fileName,fileDescription);
    } else {
        emit upLoadResultMsg(new TransferMessage("Error connecting to the server",-1));
    }
}
