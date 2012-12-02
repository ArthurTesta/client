#include "core.h"
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

bool Core::sendFile(QString * fileName,QString * fileDescription) throw (Exception){
    if(initConnection(0)){
        try{
            QFile * myFile = new QFile(*fileName);
            int fileSize = myFile->size();
            myFile->open(QIODevice::ReadOnly);
            writeQStringSock(*fileName,&uploadSocket);
            writeQStringSock(*fileDescription,&uploadSocket);
            writeIntSock(&fileSize,&uploadSocket);
            uploadSocket.write(myFile->readAll());
            uploadSocket.flush();
            uploadSocket.waitForReadyRead(10000);
            QString result = readQStringSock(&uploadSocket);
            if(result.toStdString() == "Everything ok"){
                qDebug() << "[CLI] - OK";
            }
            else {
                qDebug() << "[CLI] - ERROR";
            }
        }catch(Exception e){
            qDebug() << "Error send file";
        }
    } else {
        throw Exception("Error connecting to upload server");
    }
}
void Core::receiveStream(QString * mediaName){
    if(initConnection(1)){
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
    } else {
        throw Exception("Error connecting to stream server");
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
    try{
        if(initConnection(0)){
            sendFile(fileName,fileDescription);
        }
    }catch(Exception e){
        qDebug() << "Error engageUpload";
    }
}
