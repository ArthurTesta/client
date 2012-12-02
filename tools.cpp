#include "tools.h"

Tools::Tools()
{
}

bool Tools::isMovie(QFileInfo *f){
    if(f){
        return f->suffix() == QString("avi")
                || f->suffix() == QString("mkv");
    }
}
QString & Tools::convertCharStoQString(char * destTmp){
    QString * dest = new QString;
    int cpt=0;
    while (destTmp[cpt]!=0){
        dest->append(destTmp[cpt]);
        cpt++;
    }
    return (*dest);
}
