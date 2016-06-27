#ifndef SQLTEST_H
#define SQLTEST_H

#include "sqlI.h"

#include <QSqlDatabase>



class sqlTest : public sqlI
{
    QSqlDatabase db;
public:
    explicit sqlTest(QObject *parent = 0)
        :sqlI(parent), db(QSqlDatabase::addDatabase("QSQLITE","in_mem_db")){}
    ~sqlTest(){if(connected) disconnect();}

    // sqlI interface
public:
    virtual void connect();
    //void query();
    virtual QAbstractItemModel *getContractsModel(bool active);
    virtual QAbstractItemModel *getPersonsModel(bool);
    virtual QMap<int, QString> getPersonsName();
    //virtual QMap<int, QString> getResidentsName(int);
    virtual int insertIntoPersons(Person *);
    virtual void selectFromPersons(Person *);
    virtual int insertIntoContracts(Contract *);
    virtual void selectFromContracts(Contract *);
    virtual void updateContract(Contract *,bool,bool,bool,bool,bool,QList<int> &,QList<int> &);
    virtual void disconnect();
};

#endif // SQLTEST_H
