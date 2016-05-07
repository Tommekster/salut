
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QSettings>
#include <QFileDialog>
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

    connect(ui->pushButton_3,SIGNAL(clicked(bool)),this,SLOT(testFn()));
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

    ui->tableView->setModel(db->getContractsModel(ui->checkBox->isChecked()));
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnWidth(2,150);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // TODO connect signals & slots ...
}

void MainWindow::testFn()
{
    qDebug() << "delam testFn" << endl;
    QMapIterator<int,QString> i(db->getPersonsName());
        while (i.hasNext()) {
            i.next();
            qDebug() << i.key() << ": "
                 << i.value() << endl;
        }
}

void MainWindow::on_pushButton_clicked()
{
    contractForm form(db,this);
    form.exec();
}
