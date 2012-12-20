#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "upload.h"
#include "uilibrary.h"
#include "core.h"
class TransferMessage;
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
    uiLibrary * uiLib;
    Core * core;
    void showUploadForm();
    void createAction();
    void refreshListFiles();

private slots:
    void showLibraryUI();
    void openFile();
    void selectFileToUpload();
    void eraseList();
    void receiveStreamResult(TransferMessage * msg);

};

#endif // MAINWINDOW_H
