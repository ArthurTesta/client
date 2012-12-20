#include "upload.h"
#include "ui_upload.h"
#include "exception.h"
#include "transfermessage.h"

#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
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
    //connect(ui->buttonSave,SIGNAL(clicked()),this,SLOT(saveButtonClicked()));
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
    fileName = file->completeBaseName();
   /* if(fileName != "")
    {
        filePath = file->absoluteFilePath();
        ui->labelFileName->setText(fileName);
        fileDescription=ui->textEditDescription->toPlainText();
        ui->buttonSave->setEnabled(true);
    }*/
}
void Upload::saveButtonClicked(){
    if(ui->lineEditTitle->text()!="" && ui->textEditDescription->toPlainText()!=""){
        emit uploadSignal(&filePath,&fileDescription);
    }else {
        QMessageBox::critical(this,"Upload",QString("Description file and file name can't be empty"));
    }
}
void Upload::receiveUploadResult(TransferMessage * msg){
    if(msg->getCode()){
        QMessageBox::critical(this,"Upload",msg->getMessage());
        updateProgress(0);
    }else {
        QMessageBox::information(this,"Upload",msg->getMessage());
    }
}
