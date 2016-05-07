#include "person.h"
#include "sqlI.h"
#include "QDebug"

void Person::save()
{
    if(isNew) {
        int id=db->insertIntoPersons(this);
        if(id<0) qDebug() << "V persons jsem si vsiml, ze jsem nic nevlozil";
        else {
            isNew=false;
            rowid=id;
        }
    }
}

Person::Person(sqlI *_db)
    : db(_db),isNew(true)
{
    
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
