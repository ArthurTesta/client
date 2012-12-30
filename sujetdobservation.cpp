#include "sujetdobservation.h"
#include "observateur.h"
#include <QtAlgorithms>

void SujetDObservation::attacher(Observateur *obs){
    if(!observateurs.isEmpty()){
        QVector<Observateur *>::iterator it = qFind(observateurs.begin(),
                                                   observateurs.end(), obs);
        if(it == observateurs.end()){
            observateurs.push_back(obs);
        }
    }else{
        observateurs.push_back(obs);
    }
    notifierChangement();
}

void SujetDObservation::detacher(Observateur *obs){
    QVector<Observateur *>::iterator it = qFind(observateurs.begin(),
                                               observateurs.end(), obs);
    if(it != observateurs.end()){
        observateurs.erase(it);
    }
}

void SujetDObservation::notifierChangement(){
    for(unsigned i=0; i<observateurs.size(); ++i){
        observateurs.at(i)->refresh(this);
    }
}
