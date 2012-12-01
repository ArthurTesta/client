#ifndef TOOLS_H
#define TOOLS_H

#include <QFileInfo>

class Tools
{
public:
    Tools();
    static bool isMovie(QFileInfo * f);
};

#endif // TOOLS_H
