#include "sqltest.h"

#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlQueryModel>

#include "person.h"

void sqlTest::connect()
{
    if(connected) return;
    //QSqlDatabase::database("in_mem_db").close();
    db.setDatabaseName(":memory:");
    if(!db.open()){
        qDebug()<<"Database neni open.\n";
        QMessageBox::warning((QWidget *) parent(), tr("Unable to open database"),
                             tr("An error occured while "
                                "opening the connection: ")+db.lastError().text());
        return;
    }
    connected=true;

    // Fill test database
    QSqlQuery q(db);
    q.exec("create table Contracts (rowid integer primary key, Code varchar unique not null, Validf date, Validt date, Active boolean, Owner integer)");
    //q.exec("create table Contracts (Code varchar unique not null, Validf date, Validt date, Active boolean, Owner integer)");
    q.exec("insert into Contracts values (1, '2014/01', '2014-09-01', '2015-05-31', 0, 1)");
    q.exec("insert into Contracts values (2, '2014/02', '2014-10-01', '2015-05-31', 0, 2)");
    q.exec("insert into Contracts values (3, '2015/01', '2015-10-01', '2016-05-31', 1, 2)");
    q.exec("insert into Contracts values (4, '2015/02', '2015-11-01', '2016-10-31', 1, 6)");

    q.exec("create table Persons (rowid integer primary key, Name varchar, Surname varchar, Address text, Email varchar, Phone varchar, Bank varchar)");
    q.exec("insert into Persons values (1,'Gustav', 'Mogg', '212 Front Street, Centreville, VA 20120', 'g.mogg@email.abc', '+420123456789', '123456789/0987')");
    q.exec("insert into Persons values (2,'Joe', 'Broom', '514 Adams Avenue, Lakeland, FL 33801', 'joebroom@gmail.co.uk', '+420987654321', '987654321/0908')");
    q.exec("insert into Persons values (3,'Madeleine', 'Lefebvre', '961 Lincoln Avenue, Sewell, NJ 08080', 'madeleine.lefebvre@gmail.com', '+420456789123', '132465798/0195')");
    q.exec("insert into Persons values (4,'Stacey', 'Janes', '679 Buttonwood Drive, Hyattsville, MD 20782', 'janess@email.co.uk', '+420789123456', '369268158/0200')");
    q.exec("insert into Persons values (5,'Herve', 'Dupont', '18 Canterbury Court, Willingboro, NJ 08046', 'hDupont@mail.at', '+420987456321', '147258369/0100')");
    q.exec("insert into Persons values (6,'Julianne', 'Dupont', '630 Brown Street, Menomonee Falls, WI 53051', 'julianned@yandex.ru', '+420987456321', '159267348/0900')");

    q.exec("create table ContractsPersons (Contract integer, Person integer, Tenant bool, VarSym integer)");
    q.exec("insert into ContractsPersons values (1, 1, 1, 141)");
    q.exec("insert into ContractsPersons values (2, 2, 1, 1421)");
    q.exec("insert into ContractsPersons values (2, 3, 0, 1422)");
    q.exec("insert into ContractsPersons values (2, 4, 0, 1423)");
    q.exec("insert into ContractsPersons values (3, 2, 1, 1511)");
    q.exec("insert into ContractsPersons values (3, 3, 0, 1512)");
    q.exec("insert into ContractsPersons values (3, 4, 0, 1513)");
    q.exec("insert into ContractsPersons values (4, 6, 1, 152)");
    q.exec("insert into ContractsPersons values (4, 5, 0, 152)");
}

QAbstractItemModel *sqlTest::getContractsModel(bool active)
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    //QSqlTableModel *model = new QSqlTableModel(this,QSqlDatabase::database("in_mem_db"));
    QString Where("");
    if(active){ // pouze aktualni
        Where = " where Contracts.Active = 1";
    }
    model->setQuery("select Contracts.rowid, Contracts.Code, Persons.Name || ' ' || Persons.Surname, "
                    "strftime('%d.%m.%Y',Contracts.Validf), strftime('%d.%m.%Y',Contracts.Validt) "
                    "from Contracts inner join Persons on Contracts.Owner == Persons.rowid"+Where,
                    db);
    //model->select();

    // Zahlavi
    // 0 ... header rowid ... will be hidden
    model->setHeaderData(1,Qt::Horizontal,tr("Contract"));
    model->setHeaderData(2,Qt::Horizontal,tr("Tenant"));
    model->setHeaderData(3,Qt::Horizontal,tr("From"));
    model->setHeaderData(4,Qt::Horizontal,tr("To"));

    if(model->lastError().type() != QSqlError::NoError){
        qDebug() << "Mam chybu v modelu. " << model->lastError().text();
        return NULL; // TODO
    }

    return model;
}

QMap<int, QString> sqlTest::getResidentsName(int contr_id)
{
    QSqlQuery q(db);
    QMap<int,QString> m;
    q.exec("select rowid, Name || ' ' || Surname from Persons");
    while(q.next()){
        m.insert(q.value(0).toInt(),q.value(1).toString());
    }
    qDebug() << q.lastError().text() << endl;
    return m;
}

int sqlTest::insertIntoPersons(Person *p)
{
    QSqlQuery q(db);
    q.exec("insert into Persons (Name,Surname,Address,Email,Phone,Bank) values "
           "('"+p->getName()+"', '"+p->getSurname()+"', '"+p->getAddress()+"', '"
           +p->getEmail()+"', '"+p->getPhone()+"', '"+p->getBank()+"')");
    if(q.numRowsAffected() <1){
        qDebug() << "insertInsertIntoPersons selhalo!";
        return -1;
    }else
        return q.lastInsertId().toInt();
}

QMap<int, QString> sqlTest::getPersonsName()
{
    QSqlQuery q(db);
    QMap<int,QString> m;
    q.exec("select rowid, Name || ' ' || Surname from Persons");
    while(q.next()){
        m.insert(q.value(0).toInt(),q.value(1).toString());
    }
    qDebug() << q.lastError().text() << endl;
    return m;
}

void sqlTest::disconnect()
{
    QSqlDatabase::database("in_mem_db",false).close();
    QSqlDatabase::removeDatabase("in_mem_db");
    connected = false;
}
