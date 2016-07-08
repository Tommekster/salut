#include "transakce.h"
#include "sqlI.h"
#include <QDebug>

void Transakce::save()
{
    if(isNew){
        int id=db->insertIntoTransakce(this);
        if(id<0) qDebug()<< "V transtion jsem si vsiml, ze jsem nic nevlozil.";
        else{
            isNew=false;
            rowid=id;
        }
    }else{
        db->updateTransakce(this,chDatum,chSum,!newRozpis.isEmpty(),removeRozpis);
    }
}

Transakce::Transakce(sqlI *_db)
    :db(_db),isNew(true),chDatum(false),chSum(false)
{}
Transakce::Transakce(sqlI *_db, int id)
    :db(_db),rowid(id),isNew(false),chDatum(false),chSum(false)
{
    db->selectFromTransakce(this);
}

Transakce *Transakce::createTransakce(sqlI *db, QDate datum, int sum, const QList<Transakce::Rozpis> &rozpis)
{
    Transakce *t=new Transakce(db);
    t->datum=datum;
    t->sum=sum;
    t->rozpis=rozpis;
    t->save();

    return t;
}

void Transakce::updDatum(QDate d)
{
    if(isNew || datum==d) return;
    datum=d;
    chDatum=true;
}

void Transakce::updSum(int i)
{
    if(isNew || sum==i) return;
    sum=i;
    chSum=true;
}

void Transakce::updRozpis(QList<Transakce::Rozpis> novy)
{
    /* Mozna spatny postup, ale ted me jiny nanapadl.
     * Nejprve zjistim, co pridavam: novy seznam se starym
     * a pote co prebyva: stary seznam s novym.
     */
    // odstranim dosavadni zmeny
    newRozpis.clear();
    removeRozpis.clear();

    // najdu nove prvky
    QList<Transakce::Rozpis>::const_iterator i;
    for (i = novy.constBegin(); i != novy.constEnd(); ++i){
        if(rozpis.indexOf(*i) == -1) // novy prvek ve starem seznamu neni
            newRozpis.append(*i); // pridame ho do novych
    }

    // najdu "prebytecne" prvky
    for (i = rozpis.constBegin(); i != rozpis.constEnd(); ++i){
        if(novy.indexOf(*i) == -1) // stary prvek neni v novem seznamu
            removeRozpis.append((*i).rowid); // pridame ho do "k odstraneni"
    }
}

void Transakce::update()
{
    save();
    chDatum=chSum=false;
    newRozpis.clear();
    removeRozpis.clear();
}
