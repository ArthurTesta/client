#ifndef OBSERVATEUR_H
#define OBSERVATEUR_H

class SujetDObservation;

class Observateur {
public:
    virtual void refresh(SujetDObservation * sdo) = 0;
    virtual ~Observateur(){
    }
};

#endif // OBSERVATEUR_H
