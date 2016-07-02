#ifndef ENERGYRECORD_H
#define ENERGYRECORD_H

#include <QDate>
#include <QList>

class sqlI;

class EnergyRecord
{
public:
    struct EletricityMeter {
        unsigned int vt;
        unsigned int nt;
        EletricityMeter(unsigned int v, unsigned int n):vt(v),nt(n){}
        bool operator==(const EletricityMeter &rhs) const{return (vt == rhs.vt && nt == rhs.nt);}
    };

private:
    sqlI *db;

    int rowid;
    QDate datum;
    QList<unsigned int> hydrometers;
    QList<unsigned int> calorimeters;
    unsigned int gasometer;
    QList<EnergyRecord::EletricityMeter> eletricity_meters;

    bool isNew;


    void save();
    bool chDatum, chHydrometers, chCalorimeters, chGasometer, chEletricityMeters;
public:
    EnergyRecord(sqlI *_db);
    EnergyRecord(sqlI *_db,int);

    static EnergyRecord *createEnergyRecord(sqlI *db,
                                            QDate datum,
                                            QList<unsigned int> hydrometers,
                                            QList<unsigned int> calorimeters,
                                            unsigned int gasometer,
                                            QList<EletricityMeter> eletricity_meters);

    static EnergyRecord *lastEnergyRecord(sqlI *db);
    static void *remove(sqlI *db,int);

    // gettry
    int getRowId()const{return rowid;}
    QDate getDatum()const{return datum;}
    QList<unsigned int> getHydrometers()const{return hydrometers;}
    QList<unsigned int> getCalorimeters()const{return calorimeters;}
    unsigned int getGasometerValue()const{return gasometer;}
    QList<EletricityMeter> getEletricityMeters()const{return eletricity_meters;}

    // settry
    void setDatum(QDate d){datum=d;}
    void setHydrometers(QList<unsigned int> l){hydrometers=l;}
    void setCalorimeters(QList<unsigned int> l){calorimeters=l;}
    void setGasometerValue(unsigned int n){gasometer=n;}
    void setEletricityMeters(QList<EletricityMeter> l){eletricity_meters=l;}

    // update
    void updDatum(QDate);
    void updHydrometers(QList<unsigned int>);
    void updCalorimeters(QList<unsigned int>);
    void updGasometerValue(unsigned int);
    void updEletricityMeters(QList<EletricityMeter>);
    void update();

};

#endif // ENERGYRECORD_H
