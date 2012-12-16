#include "uilibrary.h"
#include "ui_uilibrary.h"

uiLibrary::uiLibrary(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::uiLibrary)
{
    ui->setupUi(this);
}

uiLibrary::~uiLibrary()
{
    delete ui;
}
