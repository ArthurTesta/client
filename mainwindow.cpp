#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFileInfo>
#include <QFileDialog>
#include <QDir>
#include "tools.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    uiUpload = 0;
    core = new Core();
}

void MainWindow::showUploadForm(){
    qDebug() << "show upload form";
    if(uiUpload == 0){
        uiUpload = new Upload();
    }
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
    delete ui;
    if(uiUpload){
        delete uiUpload;
        uiUpload = 0;
    }

    if(core){
        delete core;
        core = 0;
    }


}
