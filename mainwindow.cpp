
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
#include "contract.h"
#include "personform.h"
#include "person.h"
#include "energyform.h"

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
    qDebug()<<"Typ databaze:"<<dbType;
    if(QString::compare(dbType,"sqlite", Qt::CaseInsensitive) == 0){
        qDebug()<<"configuration: SQLite";
        dbFile = s.value("database/file","").toString();
        if(dbFile.isEmpty()){
            dbFile = QFileDialog::getSaveFileName(this,tr("Vyber soubor databáze"),QDir::homePath());
            if(dbFile.isEmpty()) dbFile="database.sqlite";
        }
    }else if(QString::compare(dbType,"sqlTest",Qt::CaseInsensitive) == 0){
        qDebug()<<"configuration: SQLtest";
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
    loadConfiguration();
    dbConnect();

    ui->setupUi(this);
    ui->comboBox_2->addItem(tr("Water"));
    ui->comboBox_2->addItem(tr("Heating"));
    ui->comboBox_2->addItem(tr("Electricity"));

    loadContracts();

    connect(ui->pushButton_3,SIGNAL(clicked(bool)),this,SLOT(testFn()));
    connect(ui->checkBox,SIGNAL(stateChanged(int)),this,SLOT(loadContracts()));
    connect(ui->checkBox_2,SIGNAL(stateChanged(int)),this,SLOT(loadPersons()));
}

MainWindow::~MainWindow()
{
    delete ui;
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
}

void MainWindow::loadPersons()
{
    if(!db->isConnected()) {
        qDebug() << "Koncim, protoze DB neni otevrena.\n";
        return;
    }

    ui->tableView_2->setModel(db->getPersonsModel(ui->checkBox_2->isChecked()));
    ui->tableView_2->setColumnHidden(0,true);
    ui->tableView_2->setColumnWidth(1,120);
    ui->tableView_2->setColumnWidth(2,180);
    ui->tableView_2->setColumnWidth(3,150);
    ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::loadEnergy()
{
    loadEnergy(ui->comboBox_2->currentIndex());
}

void MainWindow::loadEnergy(int type)
{
    if(!db->isConnected()) {
        qDebug() << "Koncim, protoze DB neni otevrena.\n";
        return;
    }

    ui->tableView_3->setModel(db->getEnergyModel(type));
    ui->tableView_3->setColumnHidden(0,true);
    //ui->tableView_3->setColumnWidth(1,120);
    ui->tableView_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::testFn()
{
    qDebug() << "delam testFn";
    QMapIterator<int,QString> i(db->getPersonsName());
        while (i.hasNext()) {
            i.next();
            qDebug() << i.key() << ": "
                 << i.value();
        }
}



void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    const QAbstractItemModel *m = index.model();
    //qDebug()<< "Rowid" << m->data(index.sibling(index.row(),0)).toString();
    int rowid=m->data(index.sibling(index.row(),0)).toInt();
    Contract *c=new Contract(db,rowid);
    contractForm f(db,c,this);
    int r=f.exec();
    if(r==f.Accepted) loadContracts();
}

void MainWindow::on_tabWidget_currentChanged(int tab)
{
    switch(tab){
    case 0: // Contracts
        loadContracts();
        break;
    case 1: // Persons
        loadPersons();
        break;
    case 3: // Energy
        loadEnergy();
        break;
    default:
        break;
    }
}

void MainWindow::on_btnAddContract_clicked()
{
    contractForm form(db,this);
    int res=form.exec();
    if(res == QDialog::Accepted) loadContracts();
}

void MainWindow::on_btnAddPerson_clicked()
{
    personForm form(db,this);
    int res=form.exec();
    if(res==form.Accepted) loadPersons();
}

void MainWindow::on_tableView_2_doubleClicked(const QModelIndex &index)
{
    const QAbstractItemModel *m = index.model();
    int rowid=m->data(index.sibling(index.row(),0)).toInt();
    Person *p=new Person(db,rowid);
    personForm f(db,p,this);
    int r=f.exec();
    if(r==f.Accepted) loadPersons();
}

void MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
    loadEnergy(index);
}

void MainWindow::on_btnAddEnergy_clicked()
{
    energyForm form(db,this);
    int res=form.exec();
    if(res==form.Accepted) loadEnergy();
}
