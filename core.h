#ifndef CORE_H
#define CORE_H

#include <QFileInfo>
#include <QList>

class Core
{
    QList<QFileInfo *> * listFiles;

public:
    Core();
    void setFile(QFileInfo *);
    void eraseList();
    ~Core();
};

#endif // CORE_H
