#ifndef PERSON_H
#define PERSON_H

#include <QString>
#include <QList>

class contract;

class person
{
    QString name;
    QString surname;
    QString address;
    QString email;
    QString phone;
    QString bank;

    bool generic;
    contract *current;
    bool tenant;

public:
    person();

    // settry
    QString getName()const{return name;}
    QString getSurname()const{return surname;}
    QString getAddress()const{return address;}
    QString getEmail()const{return email;}
    QString getPhone()const{return phone;}
    QString getBank()const{return bank;}

    QList<contract> getAllContracts()const;
    bool isGeneric()const{return generic;}
    contract *getCurrContract()const;
    bool isTenant()const{if(!generic) return tenant; else return false;}
};

#endif // PERSON_H
