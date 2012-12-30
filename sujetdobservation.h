#ifndef SUJETDOBSERVATION_H
#define SUJETDOBSERVATION_H

#include <QVector>

class Observateur;

class SujetDObservation
{
public:
    virtual ~SujetDObservation(){
    }
    virtual void attacher(Observateur * obs);
    virtual void detacher(Observateur * obs);

protected:
    SujetDObservation(){
    }
    virtual void notifierChangement();
    QVector<Observateur *> observateurs;
};

#endif // SUJETDOBSERVATION_H
