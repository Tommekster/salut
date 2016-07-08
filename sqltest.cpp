#include "sqltest.h"

#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlQueryModel>
#include <QSqlRecord>

#include "person.h"
#include "contract.h"
#include "energyrecord.h"
#include "transakce.h"

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

    q.exec("create table Flats (rowid integer primary key, Code varchar unique not null, VM integer, CM integer, EcM integer)");
    q.exec("insert into Flats values (1, 'Byt 1', 4, 1, 2)");
    q.exec("insert into Flats values (2, 'Byt 2', 2, 4, 3)");
    q.exec("insert into Flats values (3, 'Byt 3', 1, 3, 4)");
    q.exec("insert into Flats values (4, 'Byt 4', 3, 2, 1)");

    q.exec("create table Contracts (rowid integer primary key, Code varchar unique not null, Validf date, Validt date, Active boolean, Owner integer, Flat integer, Archived boolean)");
    //q.exec("create table Contracts (Code varchar unique not null, Validf date, Validt date, Active boolean, Owner integer)");
    q.exec("insert into Contracts values (1, '2014/01', '2014-09-01', '2015-05-31', 0, 1, 1, 0)");
    q.exec("insert into Contracts values (2, '2014/02', '2014-10-01', '2015-05-31', 0, 2, 2, 0)");
    q.exec("insert into Contracts values (3, '2015/01', '2015-10-01', '2016-05-31', 1, 2, 2, 0)");
    q.exec("insert into Contracts values (4, '2015/02', '2015-11-01', '2016-10-31', 1, 6, 4, 0)");

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

    q.exec("create table Energy (Datum Date, WM0 integer, WM1 integer, WM2 integer, WM3 integer, WM4 integer, WMS integer, "
           "Gas integer, CM1 integer, CM2 integer, CM3 integer, CM4 integer, "
           "VT1 integer, NT1 integer, VT2 integer, NT2 integer, VT3 integer, NT3 integer, VT4 integer, NT4 integer)");
    q.exec("INSERT INTO `Energy` VALUES ('2015-09-01','3','76','64','65','118','140','5334','19001','6579','7621','9376','822','1056','2761','3721','520','2726','1381','2312')");
    q.exec("INSERT INTO `Energy` VALUES ('2015-09-28','3','88','67','70','126','167','5366','19030','6579','7759','9399','860','1177','2843','3901','543','2859','1544','2520')");
    q.exec("INSERT INTO `Energy` VALUES ('2015-10-31','3','97','72','77','135','197','5702','19118','7242','8664','9815','898','1385','2934','4134','576','3060','1664','2790')");
    q.exec("INSERT INTO `Energy` VALUES ('2015-11-29','3','105','77','84','143','223','6059','20726','7752','9437','10419','936','1596','3037','4350','612','3243','1827','3025')");
    q.exec("INSERT INTO `Energy` VALUES ('2015-12-31','3','116','81','90','150','251','6520','22200','8404','10450','11064','982','1775','3128','4557','648','3429','1995','3317')");
    q.exec("INSERT INTO `Energy` VALUES ('2016-02-07','3','125','87','99','159','283','7377','24938','9873','11860','12517','1085','2034','3282','4851','694','3690','2170','3648')");
    q.exec("INSERT INTO `Energy` VALUES ('2016-03-03','3','131','91','104','165','303','7803','26327','10427','12408','13456','1158','2152','3383','5037','727','3861','2267','3865')");
    q.exec("INSERT INTO `Energy` VALUES ('2016-03-27','3','136','94','109','171','325','8190','27537','10981','13012','14186','1235','2306','3475','5203','758','4010','2352','4075')");
    q.exec("INSERT INTO `Energy` VALUES ('2016-05-07','3','146','106','120','181','371','8556','28136','11537','13365','14908','1325','2592','3615','5510','814','4324','2487','4378')");

    q.exec("create table Transtion (rowid integer primary key, Datum date, Sum integer)");
    q.exec("insert into Transtion (rowid,Datum,Sum) values (1,'2016-07-01',3500),(2,'2016-07-14',3500)");

    q.exec("CREATE TABLE Finance (transID integer, Account varchar, Amount integer, Notice varchar);");
    q.exec("INSERT INTO Finance (transID,Account,Amount,Notice) VALUES (1,'Najem',2000,'Najem kdosi'),(1,'Zalohy',1500,'Zaloha kdosi'),"
           "(2,'Najem',2000,'Najem cosi'),(2,'Zalohy',1500,'zaloha cosi');");

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

QAbstractItemModel *sqlTest::getEnergyModel(int i)
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    //QString Where("");
    QStringList l;
    l << "rowid" << "Datum";
    switch(i){
    case 0:
        l<< "WM0" << "WM1" << "WM2" << "WM3" << "WM4" << "WMS" ;
        break;
    case 1:
        l<< "Gas" << "CM1" << "CM2" << "CM3" << "CM4";
        break;
    case 2:
        l<< "VT1" << "NT1" << "VT2" << "NT2" << "VT3" << "NT3" << "VT4" << "NT4";
        break;
    default:
        break;
    }

    model->setQuery("select "+l.join(",")+" from Energy",db);

    // Zahlavi
    // 0 ... header rowid ... will be hidden
    model->setHeaderData(1,Qt::Horizontal,tr("Date"));
    /*switch(i){
    case 0:
        break;
    case 1:
        break;
    case 2:
        break;
    default:
        break;
    }*/

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
        if(name)    q.bindValue(":name",    p->getName());
        if(surname) q.bindValue(":surname", p->getSurname());
        if(bdate)   q.bindValue(":bdate",   p->getBirthDate());
        if(address) q.bindValue(":addr",    p->getAddress());
        if(email)   q.bindValue(":email",   p->getEmail());
        if(phone)   q.bindValue(":phone",   p->getPhone());
        if(bank)    q.bindValue(":bank",    p->getBank());
        q.exec();
    }
}

int sqlTest::insertIntoContracts(Contract *c)
{
    QSqlQuery q(db);
    q.prepare("insert into Contracts (Code,Validf,Validt,Active,Owner,Flat,Archived)"
              "values (:code,:vfrom,:vto,:valid,:own,:flat,0)");
    q.bindValue(":code",  c->getCode());
    q.bindValue(":vfrom", c->getFrom());
    q.bindValue(":vto",   c->getTo());
    q.bindValue(":valid", c->isValid());
    q.bindValue(":own",   c->getOwnerId());
    q.bindValue(":flat",  c->getFlatId());
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
    q.prepare("select Code,Validf,Validt,Active,Owner,Flat from Contracts where rowid=:id");
    q.bindValue(":id",c->getRowId());
    q.exec();

    if(q.first()){
        c->setCode(q.value("Code").toString());
        c->setFrom(q.value("Validf").toDate());
        c->setTo(q.value("Validt").toDate());
        c->setValid(q.value("Active").toBool());
        c->setOwnerId(q.value("Owner").toInt());
        c->setFlatId(q.value("Flat").toInt());

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

void sqlTest::updateContract(Contract *c, bool code, bool from, bool to, bool valid, bool ownId, bool flatId, const QList<int> &add, const QList<int> &remove)
{
    if(code || from || to || valid || ownId){
        QSqlQuery q(db);
        QStringList set;

        if(code) set << "code=:code";
        if(from) set << "Validf=:vfrom";
        if(to) set << "Validt=:vto";
        if(valid) set << "Active=:valid";
        if(ownId) set << "Owner=:own";
        if(flatId) set << "Flat=:flat";

        q.prepare("update Contracts set "+set.join(",")+" where rowid=:id");
        q.bindValue(":id",    c->getRowId());
        if(code)    q.bindValue(":code",  c->getCode());
        if(from)    q.bindValue(":vfrom", c->getFrom());
        if(to)      q.bindValue(":vto",   c->getTo());
        if(valid)   q.bindValue(":valid", c->isValid());
        if(ownId)   q.bindValue(":own",   c->getOwnerId());
        if(flatId)  q.bindValue(":flat",  c->getFlatId());
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

int sqlTest::insertIntoEnergy(EnergyRecord *r)
{
    QSqlQuery q(db);
    q.prepare("insert into Energy (Datum, WM0, WM1, WM2, WM3, WM4, WMS, "
              "Gas, CM1, CM2, CM3, CM4, VT1, NT1, VT2, NT2, VT3, NT3, VT4, NT4) values "
              "(:datum,:wm0,:wm1,:wm2,:wm3,:wm4,:wms,:gs,:cm1,:cm2,:cm3,:cm4,"
              ":vt1,:nt1,:vt2,:nt2,:vt3,:nt3,:vt4,:nt4)");

    q.bindValue(":datum",r->getDatum());

    QList<unsigned int> hydrometers = r->getHydrometers();
    if(hydrometers.length()<6) return -2;
    q.bindValue(":wm0",hydrometers.at(0));
    q.bindValue(":wm1",hydrometers.at(1));
    q.bindValue(":wm2",hydrometers.at(2));
    q.bindValue(":wm3",hydrometers.at(3));
    q.bindValue(":wm4",hydrometers.at(4));
    q.bindValue(":wms",hydrometers.at(5));

    QList<unsigned int> calorimeters = r->getCalorimeters();
    if(calorimeters.length()<4) return -3;
    q.bindValue(":gs",r->getGasometerValue());
    q.bindValue(":cm1",calorimeters.at(0));
    q.bindValue(":cm2",calorimeters.at(1));
    q.bindValue(":cm3",calorimeters.at(2));
    q.bindValue(":cm4",calorimeters.at(3));

    QList<EnergyRecord::EletricityMeter> eletricity_meters = r->getEletricityMeters();
    if(eletricity_meters.length()<4) return -4;
    q.bindValue(":vt1",eletricity_meters.at(0).vt);
    q.bindValue(":nt1",eletricity_meters.at(0).nt);
    q.bindValue(":vt2",eletricity_meters.at(1).vt);
    q.bindValue(":nt2",eletricity_meters.at(1).nt);
    q.bindValue(":vt3",eletricity_meters.at(2).vt);
    q.bindValue(":nt3",eletricity_meters.at(2).nt);
    q.bindValue(":vt4",eletricity_meters.at(3).vt);
    q.bindValue(":nt4",eletricity_meters.at(3).nt);

    q.exec();

    if(q.numRowsAffected() <1){
        qDebug() << "insertIntoPersons selhalo!";
        return -1;
    }else
        return q.lastInsertId().toInt();
}

void sqlTest::fillEnergyRecord(QSqlQuery &q, EnergyRecord *r)
{
    if(q.first()){
        r->setDatum(q.value("Datum").toDate());

        QList<unsigned int> hydrometers;
        hydrometers << q.value("WM0").toInt()
                    << q.value("WM1").toInt()
                    << q.value("WM2").toInt()
                    << q.value("WM3").toInt()
                    << q.value("WM4").toInt()
                    << q.value("WMS").toInt();
        r->setHydrometers(hydrometers);

        QList<unsigned int> calorimeters;
        calorimeters << q.value("CM1").toInt()
                     << q.value("CM2").toInt()
                     << q.value("CM3").toInt()
                     << q.value("CM4").toInt();
        r->setCalorimeters(calorimeters);
        r->setGasometerValue(q.value("Gas").toInt());

        QList<EnergyRecord::EletricityMeter> eletricity_meters;
        eletricity_meters << EnergyRecord::EletricityMeter(q.value("VT1").toInt(),q.value("NT1").toInt())
                          << EnergyRecord::EletricityMeter(q.value("VT2").toInt(),q.value("NT2").toInt())
                          << EnergyRecord::EletricityMeter(q.value("VT3").toInt(),q.value("NT3").toInt())
                          << EnergyRecord::EletricityMeter(q.value("VT4").toInt(),q.value("NT4").toInt());
        r->setEletricityMeters(eletricity_meters);
    }else{
        qDebug() << "fillEnergyRecord: Zaznam nebyl nalezen!";
    }
}

void sqlTest::lastEnergyRecord(EnergyRecord *r)
{
    QSqlQuery q(db);
    q.prepare("select Datum, WM0, WM1, WM2, WM3, WM4, WMS, "
              "Gas, CM1, CM2, CM3, CM4, VT1, NT1, VT2, NT2, VT3, NT3, VT4, NT4 "
              "from Energy order by Datum desc limit 1");
    q.exec();
    fillEnergyRecord(q,r);
}

void sqlTest::selectFromEnergy(EnergyRecord *r)
{
    QSqlQuery q(db);
    q.prepare("select Datum, WM0, WM1, WM2, WM3, WM4, WMS, "
              "Gas, CM1, CM2, CM3, CM4, VT1, NT1, VT2, NT2, VT3, NT3, VT4, NT4 "
              "from Energy where rowid=:rowid");
    q.bindValue(":rowid",r->getRowId());
    q.exec();
    fillEnergyRecord(q,r);
}

void sqlTest::deleteEnergyRecord(EnergyRecord *r)
{
    QSqlQuery q(db);
    q.prepare("delete from Energy where rowid=:rowid");
    q.bindValue(":rowid",r->getRowId());
    q.exec();
}

void sqlTest::updateEnergy(EnergyRecord *r, bool datum, bool hm, bool cm, bool gas, bool elm)
{
    QSqlQuery q(db);
    QStringList set;

    if(datum) set << "Datum=:datum";
    if(hm){
        set << "WM0=:wm0"
            << "WM1=:wm1"
            << "WM2=:wm2"
            << "WM3=:wm3"
            << "WM4=:wm4"
            << "WMS=:wms";
    }
    if(cm){
        set << "CM1=:cm1"
            << "CM2=:cm2"
            << "CM3=:cm3"
            << "CM4=:cm4";
    }
    if(gas) set << "Gas=:gs";
    if(elm){
        set << "VT1=:vt1" << "NT1=:nt1"
            << "VT2=:vt2" << "NT2=:nt2"
            << "VT3=:vt3" << "NT3=:nt3"
            << "VT4=:vt4" << "NT4=:nt4";
    }

    q.prepare("update Energy set "+set.join(",")+" where rowid=:id");
    q.bindValue(":id",    r->getRowId());
    if(datum) q.bindValue(":datum",r->getDatum());
    if(hm){
        QList<unsigned int> hydrometers = r->getHydrometers();
        //if(hydrometers.length()<6) return -2;
        q.bindValue(":wm0",hydrometers.at(0));
        q.bindValue(":wm1",hydrometers.at(1));
        q.bindValue(":wm2",hydrometers.at(2));
        q.bindValue(":wm3",hydrometers.at(3));
        q.bindValue(":wm4",hydrometers.at(4));
        q.bindValue(":wms",hydrometers.at(5));
    }
    if(cm){
        QList<unsigned int> calorimeters = r->getCalorimeters();
        //if(calorimeters.length()<4) return;
        q.bindValue(":cm1",calorimeters.at(0));
        q.bindValue(":cm2",calorimeters.at(1));
        q.bindValue(":cm3",calorimeters.at(2));
        q.bindValue(":cm4",calorimeters.at(3));
    }
    if(gas)
        q.bindValue(":gs",r->getGasometerValue());
    if(elm){
        QList<EnergyRecord::EletricityMeter> eletricity_meters = r->getEletricityMeters();
        //if(eletricity_meters.length()<4) return;
        q.bindValue(":vt1",eletricity_meters.at(0).vt);
        q.bindValue(":nt1",eletricity_meters.at(0).nt);
        q.bindValue(":vt2",eletricity_meters.at(1).vt);
        q.bindValue(":nt2",eletricity_meters.at(1).nt);
        q.bindValue(":vt3",eletricity_meters.at(2).vt);
        q.bindValue(":nt3",eletricity_meters.at(2).nt);
        q.bindValue(":vt4",eletricity_meters.at(3).vt);
        q.bindValue(":nt4",eletricity_meters.at(3).nt);
    }
    q.exec();
}

int sqlTest::insertIntoTransakce(Transakce *t)
{
    QSqlQuery q(db);
    q.prepare("insert into Transtion (Datum,Sum) values (:date,:sum)");
    q.bindValue(":date",t->getDatum());
    q.bindValue(":sum",t->getSum());
    q.exec();

    if(q.numRowsAffected() <1){
        qDebug() << "insertInsertIntoContracts selhalo!";
        return -1;
    }else{
        int rowid = q.lastInsertId().toInt();

        // Nakonec ke smlouve prilozime residenty
        QList<Transakce::Rozpis> add=t->getRozpis();
        QList<Transakce::Rozpis>::const_iterator i;
        for (i = add.constBegin(); i != add.constEnd(); ++i){
            q.prepare("insert into Finance (transID,Account,Amount,Notice) values (:trans, :account, :amount, :notice)");
            q.bindValue(":trans",rowid);
            q.bindValue(":account",(*i).konto);
            q.bindValue(":amount",(*i).amount);
            q.bindValue(":notice",(*i).notice);
            q.exec();
        }

        return rowid;
    }
}

void sqlTest::updateTransakce(Transakce *t, bool datum, bool sum, const QList<Transakce::Rozpis> &add, const QList<int> &remove)
{
    if(datum || sum){
        QSqlQuery q(db);
        QStringList set;

        if(datum) set << "Datum=:date";
        if(sum) set << "Sum=:sum";

        q.prepare("update Transtion set "+set.join(",")+" where rowid=:id");
        q.bindValue(":id",t->getRowId());
        if(datum) q.bindValue(":date",t->getDatum());
        if(sum)     q.bindValue(":sum",t->getSum());
        q.exec();
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

QMap<int, QString> sqlTest::getFlatsName()
{
    QSqlQuery q(db);
    QMap<int,QString> m;
    q.exec("select rowid, Code from Flats");
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
