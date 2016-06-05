#include "sqlI.h"
#include <QDebug>
#include "contract.h"

void Contract::save()
{
    if(isNew){
        int id=db->insertIntoContracts(this);
        if(id<0) qDebug()<< "V contract jsem si vsiml, ze jsem nic nevlozil.";
        else{
            isNew=false;
            rowid=id;
        }
    }
}

Contract::Contract(sqlI *_db)
    : db(_db), owner(NULL), isNew(true)
{}

Contract::Contract(sqlI *_db,int id)
    : db(_db), owner(NULL), rowid(id), isNew(false)
{
    db->selectFromContracts(this);
}

Contract *Contract::createContract(sqlI *db,
                                   const QString &code,
                                   const QDate &vfrom,
                                   const QDate &vto,
                                   bool valid, int ownId,
                                   const QList<int> &residents)
{
    Contract *c=new Contract(db);
    c->code=code;
    c->vFrom=vfrom;
    c->vTo=vto;
    c->valid=valid;
    c->ownerId=ownId;
    c->residents=residents;
    c->save();

    return c;
}

void Contract::addResidents(QList<int> &l)
{
    residents.append(l);
}

