#include "core.h"

#include <QDebug>

Core::Core()
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
