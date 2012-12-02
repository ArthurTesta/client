#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "upload.h"
#include "core.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    Upload * uiUpload;
    Core * core;

private slots:
    void showUploadForm();
    void selectFile();
    void eraseList();
    void createAction();

};

#endif // MAINWINDOW_H
