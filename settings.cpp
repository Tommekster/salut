#include "settings.h"

#include <QSettings>
#include <QDebug>
#include <QFileDialog>

settings::settings(QObject *parent) : QObject(parent), s("config.ini", QSettings::IniFormat)
{
    /*s.beginGroup("Values");
    s.setValue("number", ui->spinBox->value());
    s.endGroup();
    s.setValue("Texts/text", ui->lineEdit->text());
    s.sync();*/

    // Database
    _dbType = s.value("database/type","sqlTest").toString();

    // SQLite
    if(QString::compare(_dbType,"sqlite", Qt::CaseInsensitive) == 0){
        qDebug()<<"configuration: SQLite\n";
        _dbFile = s.value("database/file","").toString();
        if(_dbFile.isEmpty()){
            dbFileDialog(qobject_cast<QWidget*>(parent));
        }
    // SQLtest
    }else if(QString::compare(_dbType,"sqlTest",Qt::CaseInsensitive) == 0){
        qDebug()<<"configuration: SQLtest\n";
        _dbFile="database.sqlite";
    }

    dphSazby << 15 << 21;
}

void settings::dbFileDialog(QWidget *w)
{
    _dbFile=QFileDialog::getSaveFileName(w,tr("Vyber soubor databáze"),QDir::homePath());
    if(_dbFile.isEmpty()) _dbFile="database.sqlite";
}

/*
 * ~QSettings to ulozi (= provede s.sync(); )
settings::~settings()
{
    s.sync();
}*/
