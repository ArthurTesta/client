#include "upload.h"
#include "ui_upload.h"

Upload::Upload(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Upload)
{
    ui->setupUi(this);
}

Upload::~Upload()
{
    delete ui;
}
