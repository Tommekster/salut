#include "person.h"
#include "sqlI.h"
#include <QDebug>

void Person::save()
{
    if(isNew) {
        int id=db->insertIntoPersons(this);
        if(id<0) qDebug() << "V persons jsem si vsiml, ze jsem nic nevlozil";
        else {
            isNew=false;
            rowid=id;
        }
    }else{
        db->updatePerson(this, chName, chSurname, chAddress, chEmail, chPhone, chBank);
    }
}

Person::Person(sqlI *_db)
    : db(_db),isNew(true),
      chName(false), chSurname(false), chAddress(false),
      chEmail(false), chPhone(false), chBank(false)
{}

Person::Person(sqlI *_db, int id)
    :db(_db),isNew(false),rowid(id),
      chName(false), chSurname(false), chAddress(false),
      chEmail(false), chPhone(false), chBank(false)
{
    db->selectFromPersons(this);
}

Person *Person::createPerson(sqlI *db,
        const QString &name,
        const QString &surname,
        const QString &address,
        const QString &email,
        const QString &phone,
        const QString &bank)
{
    Person *p = new Person(db);
    p->name=name;
    p->surname=surname;
    p->address=address;
    p->email=email;
    p->phone=phone;
    p->bank=bank;
    p->save();

    return p;
}

void Person::updName(QString s)
{
    if(isNew) return; // jeste neni v databazi
    if(name.compare(s) == 0) return; // neni zmena

    name=s;
    chName=true;
}

void Person::updSurname(QString s)
{
    if(isNew) return; // jeste neni v databazi
    if(surname.compare(s) == 0) return; // neni zmena

    surname=s;
    chSurname=true;
}

void Person::updAddress(QString s)
{
    if(isNew) return; // jeste neni v databazi
    if(address.compare(s) == 0) return; // neni zmena

    address=s;
    chAddress=true;
}

void Person::updEmail(QString s)
{
    if(isNew) return; // jeste neni v databazi
    if(email.compare(s) == 0) return; // neni zmena

    email=s;
    chEmail=true;
}

void Person::updPhone(QString s)
{
    if(isNew) return; // jeste neni v databazi
    if(phone.compare(s) == 0) return; // neni zmena

    phone=s;
    chPhone=true;
}

void Person::updBank(QString s)
{
    if(isNew) return; // jeste neni v databazi
    if(bank.compare(s) == 0) return; // neni zmena

    bank=s;
    chBank=true;
}

void Person::update(){
    save();

    chName=false;
    chSurname=false;
    chAddress=false;
    chEmail=false;
    chPhone=false;
    chBank=false;
}
