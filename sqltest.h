#ifndef SQLTEST_H
#define SQLTEST_H

#include "sqlI.h"



class sqlTest : public sqlI
{
public:
    explicit sqlTest(QObject *parent = 0):sqlI(parent){}
    ~sqlTest(){if(connected) disconnect();}

    // sqlI interface
public:
    void connect();
    void query();
    void disconnect();
};

#endif // SQLTEST_H
