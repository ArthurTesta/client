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
    friend class MainWindow;
    explicit uiLibrary(QWidget *parent = 0);
    ~uiLibrary();
    
private:
    Ui::uiLibrary *ui;
    void search(QString);

private slots:
    void enterKey(QString);
    void searchProcess();
    void receiveSearchResult(QList < QString > * listMediaAlike);
signals :
    void sendSearchRequest(QString * fileName);
};

#endif // UILIBRARY_H
