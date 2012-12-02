#include "upload.h"
#include "ui_upload.h"
#include <QFileDialog>
#include "exception.h"
Upload::Upload(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Upload),progress(0)
{
    ui->setupUi(this);
    createAction();
}

Upload::~Upload()
{
    delete ui;
}
void Upload::createAction(){
    connect(ui->pushButton_2,SIGNAL(clicked()), this, SLOT(file()));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(saveButtonClicked()));
}
void Upload::updateProgress(qint64 numBytes){
    progress += numBytes;
    ui->progressBar->setValue(progress);
}
void Upload::file(){
    QFileInfo * file = new QFileInfo(
                QFileDialog::getOpenFileName(
                    this,
                    "Sélectrionnez un fichier à ouvrir",
                    QDir::currentPath()
                    ));
    fileName = file->baseName();
    if(fileName != "")
    {
        ui->label->setText(QFileInfo(fileName).completeBaseName());
        ui->pushButton->setEnabled(true);
    }
}
void Upload::saveButtonClicked(){
    if(ui->lineEdit->text()!="" && ui->textEdit->toPlainText()!=""){
        emit uploadSignal(&fileName,&fileDescription);
    }else {
        throw Exception("Filename or description is empty");
    }
}
