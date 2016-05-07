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
    QMap<int, QString> getPersonsName();
    void disconnect();
};

#endif // SQLTEST_H
