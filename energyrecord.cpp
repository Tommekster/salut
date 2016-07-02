#include "energyrecord.h"
#include "sqlI.h"
#include <QDebug>

void EnergyRecord::save()
{
    if(isNew){
        int id=db->insertIntoEnergy(this);
        if(id<0) qDebug()<< "V contract jsem si vsiml, ze jsem nic nevlozil.";
        else{
            isNew=false;
            rowid=id;
        }
    }
}

EnergyRecord::EnergyRecord(sqlI *_db)
    :db(_db),isNew(true)
{}

EnergyRecord::EnergyRecord(sqlI *_db, int id)
    :db(_db),isNew(false),rowid(id)
{}

EnergyRecord *EnergyRecord::createEnergyRecord(sqlI *db,
                                               QDate datum,
                                               QList<unsigned int> hydrometers,
                                               QList<unsigned int> calorimeters,
                                               unsigned int gasometer,
                                               QList<EnergyRecord::EletricityMeter> eletricity_meters)
{
    EnergyRecord *r = new EnergyRecord(db);
    r->setDatum(datum);
    r->setHydrometers(hydrometers);
    r->setCalorimeters(calorimeters);
    r->setGasometerValue(gasometer);
    r->setEletricityMeters(eletricity_meters);
    r->save();

    return r;
}

EnergyRecord *EnergyRecord::lastEnergyRecord(sqlI *db)
{
    EnergyRecord *r = new EnergyRecord(db);
    db->lastEnergyRecord(r);
    return r;
}
