#ifndef UILIBRARY_H
#define UILIBRARY_H

#include <QWidget>
#include <QString>

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
    void search(QString);

private slots:
    void enterKey(QString);
    void searchProcess();
};

#endif // UILIBRARY_H
