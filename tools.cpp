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
