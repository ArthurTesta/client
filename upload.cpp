#include "upload.h"
#include "ui_upload.h"

Upload::Upload(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Upload),progress(0)
{
    ui->setupUi(this);
}

Upload::~Upload()
{
    delete ui;
}
void Upload::createAction(){

}
void Upload::updateProgress(qint64 numBytes){
    progress += numBytes;
    ui->progressBar->setValue(progress);
}
