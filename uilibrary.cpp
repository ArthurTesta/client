#include "uilibrary.h"
#include "ui_uilibrary.h"
#include <QDebug>

uiLibrary::uiLibrary(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::uiLibrary)
{
    ui->setupUi(this);
    ui->listSearch->hide();
}

void uiLibrary::search(QString str){
    if(!str.isEmpty()){
        // effectuer le search sur la DB pour avoir un résultat (de type média)
        //core->search(str);
        emit sendSearchRequest(&str);
        qDebug() << "search : " << str;
    }
}

void uiLibrary::enterKey(QString str){
    if(str.isEmpty())
        ui->listSearch->hide();
    else{
        /**
         * si afficher : search en DB les recherches équivalentes les plus
         * pertinentes (via le nombre de hit de la recherche et des caractères
         * ressemblant)
         */
        ui->listSearch->show();
        searchProcess();
    }
}

void uiLibrary::searchProcess(){
    search(ui->search->text());
}
void uiLibrary::receiveSearchResult(QList < Media > * listMediaAlike){
    for(QList<Media>::const_iterator it = listMediaAlike->begin();
        it != listMediaAlike->end(); it++){
        qDebug() << static_cast<Media>(*it).getTitle();
    }
}

uiLibrary::~uiLibrary()
{
    delete ui;
}
