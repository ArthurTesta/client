#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    uiUpload = 0;
}

void MainWindow::connects(){
    connect(ui->actionQuit, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
}

void MainWindow::showUploadForm(){
    qDebug() << "show upload form";
    if(uiUpload == 0){
        uiUpload = new Upload();
    }
    uiUpload->show();
}

MainWindow::~MainWindow()
{
    delete ui;
    if(uiUpload != 0){
        delete uiUpload;
        uiUpload = 0;
    }
}
