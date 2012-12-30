#include "core.h"
#include "transfermessage.h"
#include "media.h"

#include <QRegExp>
#include <QDebug>
#include <stdlib.h>


Core::Core():streamPort(4001),uploadPort(4000),searchPort(4002),serverIP("127.0.0.1")
{
    listFiles = new QList<QFileInfo *>();
}

void Core::setFile(QFileInfo *f){
    if(f){
        listFiles->insert(listFiles->begin(), f);
    }
    // traitement possible du fichier pour lecture
    notifierChangement();
}

void Core::eraseList(){
    for(int i=0; i<listFiles->size(); i++){
        delete listFiles->at(i); // utiliser erase ?
    }
    listFiles->erase(listFiles->begin(), listFiles->end());
    notifierChangement();
}

QList<QFileInfo*> * Core::getList(){
    return listFiles;
}

Core::~Core(){
    delete listFiles; // quid de la r�f�rence renvoy�e via le getList() ?
}

void Core::createAction(){

}

/**
 * @brief Core::sendFile possibilit� d'update l'app pendant le traitement ?
 * @param completeFileName
 * @param fileDescription
 */
void Core::sendFile(QString * completeFileName,QString * fileDescription) throw (Exception){
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
        emit transferMsg(new TransferMessage(resultMSg,resultCode));
        uploadSocket.close();
    }catch(Exception e){
        emit transferMsg(new TransferMessage(e.what(),-1));
    }
}

/**
 * @brief Core::receiveStream possibilit� d'update l'app pendant le traitement ?
 * @param mediaName
 */
void Core::receiveStream(QString * mediaName){
    try {
        QRegExp regExp("/");
        QString fileName = mediaName->remove(0,regExp.lastIndexIn((*mediaName))+1);
        writeQStringSock(fileName,&streamSocket);
        streamSocket.flush();
        streamSocket.waitForReadyRead(10000);
        int resultCode=-1;
        readIntSock(&resultCode,&uploadSocket);
        QString resultMsg = readQStringSock(&uploadSocket);
        emit transferMsg(new TransferMessage(resultMsg,resultCode));
        streamSocket.close();
    } catch (Exception e){
        emit transferMsg(new TransferMessage(e.what(),-1));
    }
}

bool Core::isSocketConnected(int type){
    if(!type){
        return uploadSocket.waitForConnected(0);
    }
    if(type==1){
        return streamSocket.waitForConnected(0);
    }
    return searchSocket.waitForConnected(0);
}

bool Core::initConnection(int type){
    bool connectionState=false;
    if(isSocketConnected(type)){
        connectionState=true;
    }else if (!type){
        uploadSocket.connectToHost(serverIP,uploadPort);
        connectionState=uploadSocket.waitForConnected(5000);
    }else if(type==1){
        streamSocket.connectToHost(serverIP,streamPort);
        connectionState=streamSocket.waitForConnected(5000);
    }else if(type==2){
        searchSocket.connectToHost(serverIP,searchPort);
        connectionState=searchSocket.waitForConnected(5000);
    }
    return connectionState;
}

qint64 Core::getFileToUploadSize(QString * filePath){
    return QFile(*filePath).size();
}

void Core::sendSearchRequest(QString * completeFileName){
    QRegExp regExp("/");
    QString fileName = completeFileName->remove(0,regExp.lastIndexIn((*completeFileName))+1);
    writeQStringSock(fileName,&searchSocket);
    searchSocket.flush();
    searchSocket.waitForReadyRead(10000);
    int mediaAlikeCount=0;
    QList < Media > listMediaAlike;
    readIntSock(&mediaAlikeCount,&searchSocket);
    for(int cpt=0;cpt<mediaAlikeCount;cpt++){
        QString title = readQStringSock(&searchSocket);
        QString synopsis = readQStringSock(&searchSocket);
        QString date = readQStringSock(&searchSocket);
        Media tmp(cpt,title,synopsis,date);
        listMediaAlike.push_back(tmp);
    }
    emit mediaAlikeList(&listMediaAlike);
    searchSocket.close();
}

void Core::engageStream(QString *fileName){
    if(initConnection(1)){
        receiveStream(fileName);
    } else {
        emit transferMsg(new TransferMessage("Error connecting to the server",-1));
    }
}

void Core::engageSearch(QString * fileName){
    if(initConnection(2)){
        sendSearchRequest(fileName);
    } else {
        emit transferMsg(new TransferMessage("Error connecting to the server",-1));
    }
}

void Core::engageUpload(QString * fileName,QString * fileDescription){
    if(initConnection(0)){
        sendFile(fileName,fileDescription);
    } else {
        emit transferMsg(new TransferMessage("Error connecting to the server",-1));
    }
}
