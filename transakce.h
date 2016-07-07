#ifndef TRANSAKCE_H
#define TRANSAKCE_H

#include <QString>
#include <QDate>
#include <QList>

class sqlI;

class Transakce
{
public:
    struct Rozpis {
        QString konto;
        int amount;
        QString notice;
        Rozpis(QString k, int a, QString n):konto(k),amount(a),notice(n){}
        //bool operator==(const Rozpis &rhs) const{return false;}
    };

private:
    sqlI *db;

    int transID;
    QDate datum;
    QList<Rozpis> rozpis;

public:
    Transakce(sqlI *_db);
};

#endif // TRANSAKCE_H
