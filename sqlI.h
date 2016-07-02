#ifndef SQLI_H
#define SQLI_H

#include <QMap>
#include <QObject>

class QAbstractItemModel;
class Person;
class Contract;
class EnergyRecord;

class sqlI : public QObject
{
    Q_OBJECT

protected:
    bool connected;

public:
    explicit sqlI(QObject *parent = 0):QObject(parent),connected(false){}
    virtual void connect() = 0;
    //virtual void query() = 0;
    virtual QAbstractItemModel *getContractsModel(bool) = 0;
    virtual QAbstractItemModel *getPersonsModel(bool) = 0;
    virtual QAbstractItemModel *getEnergyModel(int) = 0;
    virtual QMap<int, QString> getPersonsName() = 0;
    //virtual QMap<int, QString> getResidentsName(int) = 0;
    virtual QMap<int, QString> getFlatsName() = 0;
    virtual int insertIntoPersons(Person *) = 0;
    virtual void selectFromPersons(Person *) = 0;
    virtual void updatePerson(Person *,bool,bool,bool,bool,bool,bool,bool) = 0;
    virtual int insertIntoContracts(Contract *) = 0;
    virtual void selectFromContracts(Contract *) = 0;
    virtual void updateContract(Contract *,bool,bool,bool,bool,bool,bool,QList<int> &,QList<int> &) = 0;
    virtual int insertIntoEnergy(EnergyRecord *) = 0;
    virtual void lastEnergyRecord(EnergyRecord *) = 0;
    virtual void selectFromEnergy(EnergyRecord *) = 0;
    virtual void deleteEnergyRecord(EnergyRecord *) = 0;
    virtual void updateEnergy(EnergyRecord *,bool,bool,bool,bool,bool) = 0;
    virtual void disconnect() = 0;

    bool isConnected() {return connected;}

signals:
    void onConnected(bool);
    void onDisconnected();

public slots:
};

/*
//get the database file with QFileDialog
QString fileName = QFileDialog::getOpenFileName(this,
 tr("Open Database"), "/home/yourhome", tr("SQLite Database Files (*.sqlite)"));

//add the new database
QSqlDatabase db = QSqlDatabase::addDatabase("SQLITE");
db.setHostName("localhost");
db.setDatabaseName(fileName);
//now your database will be stored in fileName

*/

#endif // SQL_H
