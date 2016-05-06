
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QSettings>
#include <QFileDialog>
#include <QSqlTableModel>
#include "sqlI.h"
#include "sqltest.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "contractform.h"

void MainWindow::loadConfiguration()
{
    QSettings s ("config.ini", QSettings::IniFormat);

    /*s.beginGroup("Values");
    s.setValue("number", ui->spinBox->value());
    s.endGroup();
    s.setValue("Texts/text", ui->lineEdit->text());
    s.sync();*/

    // Database
    QString dbType = s.value("database/type","sqlTest").toString();
    qDebug()<<dbType<<endl;
    if(QString::compare(dbType,"sqlite", Qt::CaseInsensitive) == 0){
        qDebug()<<"configuration: SQLite\n";
        dbFile = s.value("database/file","").toString();
        if(dbFile.isEmpty()){
            dbFile = QFileDialog::getSaveFileName(this,tr("Vyber soubor databáze"),QDir::homePath());
            if(dbFile.isEmpty()) dbFile="database.sqlite";
        }
    }else if(QString::compare(dbType,"sqlTest",Qt::CaseInsensitive) == 0){
        qDebug()<<"configuration: SQLtest\n";
        dbFile="database.sqlite";
    }
}

void MainWindow::dbConnect()
{
    db = new sqlTest(this);
    db->connect();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loadConfiguration();
    dbConnect();

    loadContracts();

    //connect(ui->pushButton_3,SIGNAL(clicked(bool)),this,SLOT(loadContracts()));
    connect(ui->checkBox,SIGNAL(stateChanged(int)),this,SLOT(loadContracts()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL3");
    db.setHostName("sql.endora.cz");
    db.setDatabaseName("datla1401368631");
    db.setUserName("datla14013686312");
    db.setPassword("tROeCx1");
    bool ok = db.open();

    if(ok)
        qDebug() << "Je to ok\n";
    else
        qDebug() << "Spojeni nic"<< db.lastError().text() <<"\n";
}

void MainWindow::loadContracts()
{
    if(!db->isConnected()) {
        qDebug() << "Koncim, protoze DB neni otevrena.\n";
        return;
    }
    QSqlQueryModel *model = new QSqlQueryModel(this);
    //QSqlTableModel *model = new QSqlTableModel(this,QSqlDatabase::database("in_mem_db"));
    QString Where("");
    if(ui->checkBox->isChecked()){ // pouze aktualni
        Where = " where Contracts.Active = 1";
    }
    model->setQuery("select Contracts.rowid, Contracts.Code, Persons.Name || ' ' || Persons.Surname, "
                    "strftime('%d.%m.%Y',Contracts.Validf), strftime('%d.%m.%Y',Contracts.Validt) "
                    "from Contracts inner join Persons on Contracts.Owner == Persons.rowid"+Where,
                    QSqlDatabase::database("in_mem_db"));
    //model->select();

    // Zahlavi
    // 0 ... header rowid ... will be hidden
    model->setHeaderData(1,Qt::Horizontal,tr("Contract"));
    model->setHeaderData(2,Qt::Horizontal,tr("Tenant"));
    model->setHeaderData(3,Qt::Horizontal,tr("From"));
    model->setHeaderData(4,Qt::Horizontal,tr("To"));

    if(model->lastError().type() != QSqlError::NoError){
        qDebug() << "Mam chybu v modelu. " << model->lastError().text();
        return; // TODO
    }
    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnWidth(2,150);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);


    // TODO connect ...
}

void MainWindow::on_pushButton_clicked()
{
    contractForm form(this);
    form.exec();
}
