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
#include <QVariant>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),uiUpload(0), uiLib(0)
{
    ui->setupUi(this);
    core = new Core();
    core->attacher(this);

    connect(ui->actionQuitter, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
}

void MainWindow::refresh(SujetDObservation *sdo){
    if(sdo == core){
        refreshListFiles();
    }
}

void MainWindow::showUploadForm(){
    if(!uiUpload){
        uiUpload = new Upload();
    }
    createAction();
    uiUpload->show();
}

void MainWindow::showLibraryUI() {
    if(!uiLib) {
        uiLib = new uiLibrary();
    }
    uiLib->setWindowModality(Qt::ApplicationModal);
    uiLib->show();
}

void MainWindow::openFile(){
    /**
     * Modifier le QFileDialog pour bloquer
     * que certain types de fichiers.
     * -> Double check sur le type de format
     */
    QFileInfo * file = new QFileInfo(
                QFileDialog::getOpenFileName(
                    this,
                    "Sélectrionnez un fichier à ouvrir",
                    QDir::currentPath(),
                    tr("All files (*.mkv *.avi);;mkv (*.mkv);;avi (*.avi")
                    ));
    if(file && Tools::isMovie(file)){
        core->setFile(file);
    }
}

void MainWindow::refreshListFiles(){
    ui->menuFichiers_r_cents->clear();
    disconnect(ui->menuFichiers_r_cents, SIGNAL(triggered(QAction*)),
               this, SLOT(test(QAction*)));
    QList<QFileInfo*>* files = core->getList();
    if(files->isEmpty()){
        ui->menuFichiers_r_cents->setEnabled(false);
    }else{
        ui->menuFichiers_r_cents->setEnabled(true);
        for(QList<QFileInfo*>::const_iterator it = files->begin();
            it != files->end(); it++){
                QFileInfo* file = static_cast<QFileInfo *>(*it);
                QAction * action = new QAction(this);
                if(file && action){
                    action->setData(file->absoluteFilePath());
                    action->setText(file->completeBaseName());
                    ui->menuFichiers_r_cents->addAction(action);
                }
        }
        QAction * action = new QAction(this);
        if(action){
            action->setText("Effacer");
            ui->menuFichiers_r_cents->addSeparator();
            ui->menuFichiers_r_cents->addAction(action);
        }
        connect(ui->menuFichiers_r_cents, SIGNAL(triggered(QAction*)),
                this, SLOT(test(QAction*)));
    }
}

void MainWindow::test(QAction *action){
    if(action)
        if(action->data().isNull())
            eraseList();
        else
            qDebug() << action->data();
}

void MainWindow::selectFileToUpload(){
    qDebug() << "select file to upload";
    QFileInfo * file = new QFileInfo(
                QFileDialog::getOpenFileName(
                    this,
                    "Sélectrionnez un fichier à ouvrir",
                    QDir::currentPath(),
                    tr("All files (*.mkv *.avi);;mkv (*.mkv);;avi (*.avi")
                    ));
    if(file && Tools::isMovie(file)){
        showUploadForm();
    }
}

void MainWindow::eraseList(){
    if(core){
        core->eraseList();
    }
    refreshListFiles();
}

/**
 * @brief connect à redéfinir
 */
void MainWindow::createAction(){
    connect(&(core->uploadSocket), SIGNAL(bytesWritten(qint64)),uiUpload,
            SLOT(updateProgress(qint64)));    //Does not work
    connect(uiUpload,SIGNAL(uploadSignal(QString*,QString*)),core,
            SLOT(engageUpload(QString*,QString*)));
    connect(core,SIGNAL(transferMsg(TransferMessage*)),uiUpload,
            SLOT(receiveUploadResult(TransferMessage*)));

    connect(core,SIGNAL(mediaAlikeList(QList<Media>*)),uiLib,
            SLOT(receiveSearchResult(QList<Media>*)));
    connect(uiLib,SIGNAL(sendSearchRequest(QString*)),core,
            SLOT(engageSearch(QString*)));

    //connect(uiLib,SIGNAL(buttonOkPushedSignal(QString*)),core,
    //      SLOT(engageStream(QString*)));
    connect(core,SIGNAL(transferMsg(TransferMessage*)),this,
            SLOT(receiveStreamResult(TransferMessage*)));
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

    if(uiLib){
        delete uiLib;
        uiLib = 0;
    }

    if(core){
        delete core;
        core = 0;
    }
}
