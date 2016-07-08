#include "transakce.h"
#include "sqlI.h"

void Transakce::save()
{
    if(isNew){
        db->insertIntoTransakce(this);
    }
}

Transakce::Transakce(sqlI *_db)
{

}

Transakce *Transakce::createTransakce(sqlI *db, QDate datum, const QList<Transakce::Rozpis> &rozpis)
{
    Transakce *t=new Transakce(db);
    t->datum=datum;
    t->rozpis=rozpis;
    t->save();
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
