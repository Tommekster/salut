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
    void connect();
    //void query();
    QAbstractItemModel *getContractsModel(bool active);
    QAbstractItemModel *getPersonsModel(bool);
    QMap<int, QString> getPersonsName();
    QMap<int, QString> getResidentsName(int);
    int insertIntoPersons(Person *);
    void selectFromPersons(Person *);
    int insertIntoContracts(Contract *);
    void selectFromContracts(Contract *);
    void disconnect();
};

#endif // SQLTEST_H
