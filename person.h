#ifndef PERSON_H
#define PERSON_H

#include <QString>
#include <QList>

class contract;
class sqlI;

class Person
{
    sqlI *db;

    QString name;
    QString surname;
    QString address;
    QString email;
    QString phone;
    QString bank;

    bool generic; // Proc to tu je?
    contract *current;
    bool tenant;

    bool isNew; // jestli uz je v databazi
    int rowid;

    void save();

public:
    Person(sqlI *_db);

    static Person *createPerson(
            sqlI *db,
            const QString &name,
            const QString &surname,
            const QString &address,
            const QString &email,
            const QString &phone,
            const QString &bank);

    // gettry
    QString getName()const{return name;}
    QString getSurname()const{return surname;}
    QString getAddress()const{return address;}
    QString getEmail()const{return email;}
    QString getPhone()const{return phone;}
    QString getBank()const{return bank;}
    int getRowid()const{return rowid;}

    // settry
    //void setName()

    QList<contract> getAllContracts()const;
    bool isGeneric()const{return generic;}
    contract *getCurrContract()const;
    bool isTenant()const{if(!generic) return tenant; else return false;}
};

#endif // PERSON_H
