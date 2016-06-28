#ifndef PERSON_H
#define PERSON_H

#include <QString>
#include <QList>
#include <QDate>

class Contract;
class sqlI;

class Person
{
    sqlI *db;

    QString name;
    QString surname;
    QDate birthDate;
    QString address;
    QString email;
    QString phone;
    QString bank;

    bool generic; // Proc to tu je?
    Contract *current;
    bool tenant;

    bool isNew; // jestli uz je v databazi
    int rowid;

    void save();

    bool chName, chSurname, chBirthDate, chAddress, chEmail, chPhone, chBank;

public:
    Person(sqlI *_db);
    Person(sqlI *_db,int);

    static Person *createPerson(
            sqlI *db,
            const QString &name,
            const QString &surname,
            const QDate &birthDate,
            const QString &address,
            const QString &email,
            const QString &phone,
            const QString &bank);

    // gettry
    QString getName()const{return name;}
    QString getSurname()const{return surname;}
    QDate   getBirthDate()const{return birthDate;}
    QString getAddress()const{return address;}
    QString getEmail()const{return email;}
    QString getPhone()const{return phone;}
    QString getBank()const{return bank;}
    int getRowId()const{return rowid;}

    // settry
    void setName(QString s){name=s;}
    void setSurname(QString s){surname=s;}
    void setBirthDate(QDate d){birthDate=d;}
    void setAddress(QString s){address=s;}
    void setEmail(QString s){email=s;}
    void setPhone(QString s){phone=s;}
    void setBank(QString s){bank=s;}

    // update
    void updName(QString);
    void updSurname(QString);
    void updBirthDate(QDate);
    void updAddress(QString);
    void updEmail(QString);
    void updPhone(QString);
    void updBank(QString);
    void update();

    QList<Contract> getAllContracts()const;
    bool isGeneric()const{return generic;} // Proc to tu je?
    Contract *getCurrContract()const;
    bool isTenant()const{if(!generic) return tenant; else return false;}
};

#endif // PERSON_H
