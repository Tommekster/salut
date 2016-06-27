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
    }else{
        db->updateContract(this,chCode,chFrom,chTo,chValid,chOwnerId,newResidents,removeResidents);
    }
}

Contract::Contract(sqlI *_db)
    : db(_db), owner(NULL), isNew(true),
      chCode(false), chFrom(false), chTo(false), chValid(false), chOwnerId(false)
{}

Contract::Contract(sqlI *_db,int id)
    : db(_db), rowid(id), owner(NULL), isNew(false),
      chCode(false), chFrom(false), chTo(false), chValid(false), chOwnerId(false)
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

void Contract::updCode(QString s)
{
    if(isNew) return; // jeste neni v databazi
    if(code.compare(s) == 0) return;

    code=s;
    chCode=true; // Code was chenged
}

void Contract::updFrom(QDate d)
{
    if(isNew) return; // jeste neni v databazi
    if(vFrom == d) return;

    vFrom=d;
    chFrom=true;
}

void Contract::updTo(QDate d)
{
    if(isNew) return; // jeste neni v databazi
    if(vTo == d) return;

    vTo=d;
    chTo=true;
}

void Contract::updValid(bool b)
{
    if(isNew) return;
    if(valid==b) return;

    valid=b;
    chValid=true;
}

void Contract::updOwnerId(int i)
{
    if(isNew) return;
    if(ownerId==i) return;

    ownerId=i;
    chOwnerId=true;
}

void Contract::updResidents(QList<int> &novy)
{
    /* Mozna spatny postup, ale ted me jiny nanapadl.
     * Nejprve zjistim, co pridavam: novy seznam se starym
     * a pote co prebyva: stary seznam s novym.
     */
    // odstranim dosavadni zmeny
    newResidents.clear();
    removeResidents.clear();

    // najdu nove prvky
    QList<int>::const_iterator i;
    for (i = novy.constBegin(); i != novy.constEnd(); ++i){
        if(residents.indexOf(*i) == -1) // novy prvek ve starem seznamu neni
            newResidents.append(*i); // pridame ho do novych
    }

    // najdu "prebytecne" prvky
    for (i = residents.constBegin(); i != residents.constEnd(); ++i){
        if(novy.indexOf(*i) == -1) // stary prvek neni v novem seznamu
            removeResidents.append(*i); // pridame ho do "k odstraneni"
    }
}

void Contract::update()
{
    save();

    chCode=false;
    chFrom=false;
    chTo=false;
    chValid=false;
    chOwnerId=false;

    newResidents.clear();
    removeResidents.clear();
}


