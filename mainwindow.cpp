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
    ui(new Ui::MainWindow),uiUpload(0)
{
    ui->setupUi(this);
    core = new Core();
}

void MainWindow::showUploadForm(){
    qDebug() << "show upload form";
    if(uiUpload == 0){
        uiUpload = new Upload();
    }
    uiUpload->setWindowModality(Qt::WindowModal);
    createAction();
    uiUpload->show();
}

void MainWindow::selectFile(){
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
    if(Tools::isMovie(file))
        core->setFile(file);
}

void MainWindow::eraseList(){
    core->eraseList();
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

void MainWindow::createAction(){
    connect(&(core->uploadSocket), SIGNAL(bytesWritten(qint64)),uiUpload, SLOT(updateProgress(qint64)));    //Does not work
    connect(uiUpload,SIGNAL(uploadSignal(QString*,QString*)),core,SLOT(engageUpload(QString*,QString*)));
    connect(core,SIGNAL(upLoadResultMsg(TransferMessage*)),uiUpload,SLOT(showUploadResult(TransferMessage*)));
}
