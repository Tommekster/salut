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
    }else{
        db->updateEnergy(this,chDatum, chHydrometers, chCalorimeters, chGasometer, chEletricityMeters);
    }
}

EnergyRecord::EnergyRecord(sqlI *_db)
    :db(_db),isNew(true),
      chDatum(false), chHydrometers(false), chCalorimeters(false), chGasometer(false), chEletricityMeters(false)
{}

EnergyRecord::EnergyRecord(sqlI *_db, int id)
    :db(_db),rowid(id),isNew(false),
      chDatum(false), chHydrometers(false), chCalorimeters(false), chGasometer(false), chEletricityMeters(false)
{
    db->selectFromEnergy(this);
}

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

void *EnergyRecord::remove(sqlI *db, int id)
{
    EnergyRecord *r = new EnergyRecord(db);
    r->rowid=id;
    db->deleteEnergyRecord(r);
    delete r;
}

void EnergyRecord::updDatum(QDate d)
{
    if(isNew || datum == d) return;

    chDatum = true;
    datum = d;
}

void EnergyRecord::updHydrometers(QList<unsigned int> l)
{
    if(isNew || hydrometers == l) return;

    chHydrometers = true;
    hydrometers = l;
}

void EnergyRecord::updCalorimeters(QList<unsigned int> l)
{
    if(isNew || calorimeters == l) return;

    chCalorimeters = true;
    calorimeters = l;
}

void EnergyRecord::updGasometerValue(unsigned int i)
{
    if(isNew || gasometer == i) return;

    chGasometer = true;
    gasometer = i;
}

void EnergyRecord::updEletricityMeters(QList<EnergyRecord::EletricityMeter> l)
{
    if(isNew || eletricity_meters == l) return;

    chEletricityMeters = true;
    eletricity_meters = l;
}

void EnergyRecord::update()
{
    save();
    chDatum = chHydrometers = chCalorimeters = chGasometer = chEletricityMeters = false;
}
