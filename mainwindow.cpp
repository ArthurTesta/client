#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tools.h"
#include "transfermessage.h"

#include <QDebug>
#include <QFileInfo>
#include <QFileDialog>
#include <QDir>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),uiUpload(0), uiLib(0)
{
    ui->setupUi(this);
    core = new Core();
}

void MainWindow::showUploadForm(){
    qDebug() << "show upload form UI";
    if(!uiUpload){
        uiUpload = new Upload();
    }
    uiUpload->setWindowModality(Qt::WindowModal);
    createAction();
    uiUpload->show();
}

void MainWindow::showLibraryUI() {
    qDebug() << "show library UI";
    if(!uiLib) {
        uiLib = new uiLibrary();
    }
    uiLib->setWindowModality(Qt::WindowModal);
    uiLib->show();
}

void MainWindow::openFile(){
    qDebug() << "open file to read";
    /**
     * Modifier le QFileDialog pour bloquer
     * que certain types de fichiers.
     * -> Double check sur le type de format
     */
    QFileInfo * file = new QFileInfo(
                QFileDialog::getOpenFileName(
                    this,
                    "Sélectrionnez un fichier à ouvrir",
                    QDir::currentPath()
                    ));
    if(file && Tools::isMovie(file))
        core->setFile(file);
}

void MainWindow::selectFileToUpload(){
    qDebug() << "select file to upload";
    QFileInfo * file = new QFileInfo(
                QFileDialog::getOpenFileName(
                    this,
                    "Sélectrionnez un fichier à ouvrir",
                    QDir::currentPath()
                    ));
    if(file && Tools::isMovie(file)){
        showUploadForm();
    }
}

void MainWindow::eraseList(){
    if(core){
        core->eraseList();
    }
}

/**
 * @brief connect à redéfinir
 */
void MainWindow::createAction(){
    connect(&(core->uploadSocket), SIGNAL(bytesWritten(qint64)),uiUpload, SLOT(updateProgress(qint64)));    //Does not work
    connect(uiUpload,SIGNAL(uploadSignal(QString*,QString*)),core,SLOT(engageUpload(QString*,QString*)));
    connect(core,SIGNAL(upLoadResultMsg(TransferMessage*)),uiUpload,SLOT(showUploadResult(TransferMessage*)));
}

MainWindow::~MainWindow()
{
    if(uiUpload){
        delete uiUpload;
        uiUpload = 0;
    }

    if(core){
        delete core;
        core = 0;
    }
}
