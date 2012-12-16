#ifndef UILIBRARY_H
#define UILIBRARY_H

#include <QWidget>

namespace Ui {
class uiLibrary;
}

class uiLibrary : public QWidget
{
    Q_OBJECT
    
public:
    explicit uiLibrary(QWidget *parent = 0);
    ~uiLibrary();
    
private:
    Ui::uiLibrary *ui;
};

#endif // UILIBRARY_H
