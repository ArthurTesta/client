#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tools.h"
#include "transfermessage.h"

#include <QDebug>
#include <QFileInfo>
#include <QFileDialog>
#include <QDir>
#include <QList>
#include <QMessageBox>

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
    //qDebug() << "show library UI";
    if(!uiLib) {
        uiLib = new uiLibrary();
    }
    uiLib->setWindowModality(Qt::WindowModal);
    uiLib->show();
}

void MainWindow::openFile(){
    //qDebug() << "open file to read";
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
    if(file && Tools::isMovie(file)){
        core->setFile(file);
        refreshListFiles();
    }
}

/**
 * fonction à modifier car il y a un duplicata des filename dans le menu
 * car on purge pas le menu avant
 * soit on purge et on add la liste + effacer
 * soit on add juste un filename en début de liste (comment ?)
 */
void MainWindow::refreshListFiles(){
    QList<QFileInfo*>* files= core->getList();
    for(QList<QFileInfo*>::const_iterator it = files->begin(); it != files->end(); it++){
        QFileInfo* file = static_cast<QFileInfo *>(*it);
        // insérer le nom de fichier dans la liste du menu
        QAction * action = new QAction(this);
        action->setObjectName(file->fileName());
        action->setText(file->completeBaseName());
        ui->menuFichiers_r_cents->addAction(action);
    }
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
    connect(core,SIGNAL(transferMsg(TransferMessage*)),uiUpload,SLOT(receiveUploadResult(TransferMessage*)));

    connect(core,SIGNAL(mediaAlikeList(QList<QString>*)),uiLib,SLOT(receiveSearchResult(QList<QString>*)));
    connect(uiLib,SIGNAL(sendSearchRequest(QString*)),core,SLOT(engageSearch(QString*)));

    //connect(uiLib,SIGNAL(buttonOkPushedSignal(QString*)),core,SLOT(engageStream(QString*)));
    connect(core,SIGNAL(transferMsg(TransferMessage*)),this,SLOT(receiveStreamResult(TransferMessage*)));
}

void MainWindow::receiveStreamResult(TransferMessage *msg){
    if(!msg->getCode()){
        //stream
    }else{
        QMessageBox::critical(this,"Stream",msg->getMessage());
    }
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
