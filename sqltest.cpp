#include "sqltest.h"

#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlQueryModel>
#include <QSqlRecord>

#include "person.h"
#include "contract.h"

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
    q.exec("create table Contracts (rowid integer primary key, Code varchar unique not null, Validf date, Validt date, Active boolean, Owner integer, Archived boolean)");
    //q.exec("create table Contracts (Code varchar unique not null, Validf date, Validt date, Active boolean, Owner integer)");
    q.exec("insert into Contracts values (1, '2014/01', '2014-09-01', '2015-05-31', 0, 1, 0)");
    q.exec("insert into Contracts values (2, '2014/02', '2014-10-01', '2015-05-31', 0, 2, 0)");
    q.exec("insert into Contracts values (3, '2015/01', '2015-10-01', '2016-05-31', 1, 2, 0)");
    q.exec("insert into Contracts values (4, '2015/02', '2015-11-01', '2016-10-31', 1, 6, 0)");

    q.exec("create table Persons (rowid integer primary key, Name varchar, Surname varchar, BirthDate date, Address text, Email varchar, Phone varchar, Bank varchar, Archived boolean)");
    q.exec("insert into Persons values (1,'Gustav', 'Mogg', '1989-09-21', '212 Front Street, Centreville, VA 20120', 'g.mogg@email.abc', '+420123456789', '123456789/0987', 0)");
    q.exec("insert into Persons values (2,'Joe', 'Broom', '1993-06-01', '514 Adams Avenue, Lakeland, FL 33801', 'joebroom@gmail.co.uk', '+420987654321', '987654321/0908', 0)");
    q.exec("insert into Persons values (3,'Madeleine', 'Lefebvre', '1965-02-03', '961 Lincoln Avenue, Sewell, NJ 08080', 'madeleine.lefebvre@gmail.com', '+420456789123', '132465798/0195', 0)");
    q.exec("insert into Persons values (4,'Stacey', 'Janes', '1978-04-09', '679 Buttonwood Drive, Hyattsville, MD 20782', 'janess@email.co.uk', '+420789123456', '369268158/0200', 0)");
    q.exec("insert into Persons values (5,'Herve', 'Dupont', '1988-05-27', '18 Canterbury Court, Willingboro, NJ 08046', 'hDupont@mail.at', '+420987456321', '147258369/0100', 0)");
    q.exec("insert into Persons values (6,'Julianne', 'Dupont', '1990-07-13', '630 Brown Street, Menomonee Falls, WI 53051', 'julianned@yandex.ru', '+420987456321', '159267348/0900', 0)");

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

QAbstractItemModel *sqlTest::getPersonsModel(bool active)
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    QString Where("");
    if(active){ // pouze s aktualni smlouvou
        Where = " where Contracts.Active=1";
    }
    //model->setQuery("select rowid, Name || ' ' || Surname, Email, Phone from Persons",db);
    /*model->setQuery("select Persons.rowid, Persons.Name || ' ' || Persons.Surname, Persons.Email, Persons.Phone "
                    "from Persons inner join Contracts on Contracts.Owner = Persons.rowid"+Where, db);*/
    // select Persons.rowid, Persons.Name || ' ' || Persons.Surname, Contracts.Code from Persons left join ContractsPersons on ContractsPersons.Person=Persons.rowid left join Contracts on (Contracts.rowid=ContractsPersons.Contract or Contracts.Owner=Persons.rowid) where Contracts.Active=1 group by Persons.rowid
    // select Persons.rowid, Persons.Name || ' ' || Persons.Surname, Persons.Email, Persons.Phone, count(Contracts.Code) from Persons left join Contracts on Persons.rowid = Contracts.Owner group by Persons.rowid
    model->setQuery("select Persons.rowid, Persons.Name || ' ' || Persons.Surname, Persons.Email, Persons.Phone from Persons "
                    "left join ContractsPersons on ContractsPersons.Person=Persons.rowid "
                    "left join Contracts on (Contracts.rowid=ContractsPersons.Contract or Contracts.Owner=Persons.rowid)"+
                    Where+" group by Persons.rowid",db);

    // Zahlavi
    // 0 ... header rowid ... will be hidden
    model->setHeaderData(1,Qt::Horizontal,tr("Name"));
    model->setHeaderData(2,Qt::Horizontal,tr("E-mail"));
    model->setHeaderData(3,Qt::Horizontal,tr("Phone"));

    if(model->lastError().type() != QSqlError::NoError){
        qDebug() << "Mam chybu v modelu: " << model->lastError().text();
        return NULL; // TODO
    }

    return model;
}

int sqlTest::insertIntoPersons(Person *p)
{
    QSqlQuery q(db);
    q.prepare("insert into Persons (Name,Surname,BirthDate,Address,Email,Phone,Bank) values "
           "(:name, :surname, :bdate, :addr, :email, :phone, :bank)");
    q.bindValue(":name",    p->getName());
    q.bindValue(":surname", p->getSurname());
    q.bindValue(":bdate",   p->getBirthDate());
    q.bindValue(":addr",    p->getAddress());
    q.bindValue(":email",   p->getEmail());
    q.bindValue(":phone",   p->getPhone());
    q.bindValue(":bank",    p->getBank());
    q.exec();

    if(q.numRowsAffected() <1){
        qDebug() << "insertIntoPersons selhalo!";
        return -1;
    }else
        return q.lastInsertId().toInt();
}

void sqlTest::selectFromPersons(Person *p)
{
    QSqlQuery q(db);
    q.prepare("select Name,Surname,BirthDate,Address,Email,Phone,Bank from Persons where rowid=:rowid");
    q.bindValue(":rowid",p->getRowId());
    q.exec();

    if(q.first()){
        p->setName(q.value("Name").toString());
        p->setSurname(q.value("Surname").toString());
        p->setBirthDate(q.value("BirthDate").toDate());
        p->setAddress(q.value("Address").toString());
        p->setEmail(q.value("Email").toString());
        p->setPhone(q.value("Phone").toString());
        p->setBank(q.value("Bank").toString());
    }else{
        qDebug() << "selectFromPersons: Zaznam nebyl nalezen!";
    }
}

void sqlTest::updatePerson(Person *p, bool name, bool surname, bool bdate, bool address, bool email, bool phone, bool bank)
{
    if(name || surname || bdate || address || email || phone || bank){
        QSqlQuery q(db);
        QStringList set;

        if(name) set << "Name=:name";
        if(surname) set << "Surname=:surname";
        if(bdate) set << "BirthDate=:bdate";
        if(address) set << "Address=:addr";
        if(email) set << "Email=:email";
        if(phone) set << "Phone=:phone";
        if(bank) set << "Bank=:bank";

        q.prepare("update Persons set "+set.join(",")+" where rowid=:id");
        q.bindValue(":id",      p->getRowId());
        q.bindValue(":name",    p->getName());
        q.bindValue(":surname", p->getSurname());
        q.bindValue(":bdate",   p->getBirthDate());
        q.bindValue(":addr",    p->getAddress());
        q.bindValue(":email",   p->getEmail());
        q.bindValue(":phone",   p->getPhone());
        q.bindValue(":bank",    p->getBank());
        q.exec();
    }
}

int sqlTest::insertIntoContracts(Contract *c)
{
    QSqlQuery q(db);
    q.prepare("insert into Contracts (Code,Validf,Validt,Active,Owner)"
              "values (:code,:vfrom,:vto,:valid,:own)");
    q.bindValue(":code",  c->getCode());
    q.bindValue(":vfrom", c->getFrom());
    q.bindValue(":vto",   c->getTo());
    q.bindValue(":valid", c->isValid());
    q.bindValue(":own",   c->getOwnerId());
    q.exec();

    if(q.numRowsAffected() <1){
        qDebug() << "insertInsertIntoContracts selhalo!";
        return -1;
    }else{
        int rowid = q.lastInsertId().toInt();

        // Nakonec ke smlouve prilozime residenty
        QList<int> add=c->getResidentsIDs();
        QList<int>::const_iterator i;
        for (i = add.constBegin(); i != add.constEnd(); ++i){
            q.prepare("insert into ContractsPersons (Contract, Person, Tenant, VarSym) values (:contract, :person, 0, 0)");
            q.bindValue(":contract", rowid);
            q.bindValue(":person", *i);
            q.exec();
        }

        return rowid;
    }
}

void sqlTest::selectFromContracts(Contract *c)
{
    QSqlQuery q(db);
    q.prepare("select Code,Validf,Validt,Active,Owner from Contracts where rowid=:id");
    q.bindValue(":id",c->getRowId());
    q.exec();

    if(q.first()){
        c->setCode(q.value("Code").toString());
        c->setFrom(q.value("Validf").toDate());
        c->setTo(q.value("Validt").toDate());
        c->setValid(q.value("Active").toBool());
        c->setOwnerId(q.value("Owner").toInt());

        // load residents
        QList<int> residents;
        q.prepare("select Person from ContractsPersons where Contract=:rowid");
        q.bindValue(":rowid",c->getRowId());
        q.exec();
        while(q.next()){
            residents << q.value("Person").toInt();
        }
        c->addResidents(residents);
    }else{
        qDebug() << "selectFromContracts: Zaznam nebyl nalezen!";
    }
}

void sqlTest::updateContract(Contract *c, bool code, bool from, bool to, bool valid, bool ownId, QList<int> &add, QList<int> &remove)
{
    if(code || from || to || valid || ownId){
        QSqlQuery q(db);
        QStringList set;

        if(code) set << "code=:code";
        if(from) set << "Validf=:vfrom";
        if(to) set << "Validt=:vto";
        if(valid) set << "Active=:valid";
        if(ownId) set << "Owner=:own";

        q.prepare("update Contracts set "+set.join(",")+" where rowid=:id");
        q.bindValue(":id",c->getRowId());
        q.bindValue(":code",  c->getCode());
        q.bindValue(":vfrom", c->getFrom());
        q.bindValue(":vto",   c->getTo());
        q.bindValue(":valid", c->isValid());
        q.bindValue(":own",   c->getOwnerId());
        q.exec();
    }
    if(!add.isEmpty()){
        QSqlQuery q(db);
        QList<int>::const_iterator i;
        for (i = add.constBegin(); i != add.constEnd(); ++i){
            q.prepare("insert into ContractsPersons (Contract, Person, Tenant, VarSym) values (:contract, :person, 0, 0)");
            q.bindValue(":contract", c->getRowId());
            q.bindValue(":person", *i);
            q.exec();
        }
    }
    if(!remove.isEmpty()){
        QSqlQuery q(db);
        QList<int>::const_iterator i;
        for (i = remove.constBegin(); i != remove.constEnd(); ++i){
            q.prepare("delete from ContractsPersons where Contract=:contract and Person=:person");
            q.bindValue(":contract", c->getRowId());
            q.bindValue(":person", *i);
            q.exec();
        }
    }
}

QMap<int, QString> sqlTest::getPersonsName()
{
    QSqlQuery q(db);
    QMap<int,QString> m;
    q.exec("select rowid, Name || ' ' || Surname from Persons");
    while(q.next()){
        m.insert(q.value(0).toInt(),q.value(1).toString());
    }
    //qDebug() << q.lastError().text() << endl;
    return m;
}

void sqlTest::disconnect()
{
    QSqlDatabase::database("in_mem_db",false).close();
    QSqlDatabase::removeDatabase("in_mem_db");
    connected = false;
}
