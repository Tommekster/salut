#ifndef CONTRACT_H
#define CONTRACT_H

#include <QString>
#include <QDate>
#include <QList>

class Person;
class sqlI;

class Contract
{
    sqlI *db;

    int rowid;
    QString code;
    QDate vFrom;
    QDate vTo;
    bool valid;
    int ownerId;
    QList<int> residents;

    Person *owner;

    bool isNew; // jestli uz je v databazi

    void save();

    bool chCode, chFrom, chTo, chValid, chOwnerId;
    QList<int> newResidents;
    QList<int> removeResidents;

public:
    Contract(sqlI *_db);
    Contract(sqlI *_db,int id);

    static Contract *createContract(sqlI *db,
            const QString &code,
            const QDate &vfrom,
            const QDate &vto,
            bool valid,
            int ownId,
            const QList<int> &residents);

    // getry
    int getRowId()const{return rowid;}
    QString getCode()const{return code;}
    QDate getFrom()const{return vFrom;}
    QDate getTo()const{return vTo;}
    bool isValid()const{return valid;}
    Person *getOwner()const;
    int getOwnerId()const{return ownerId;}
    QList<Person> getResidents();
    QList<int> getResidentsIDs()const{return residents;}

    // setry
    void setCode(QString s){code=s;}
    void setFrom(QDate d){vFrom=d;}
    void setTo(QDate d){vTo=d;}
    void setValid(bool b){valid=b;}
    void setOwnerId(int i){ownerId=i;}
    void addResidents(QList<int> &l){residents.append(l);}

    // update
    void updCode(QString);
    void updFrom(QDate);
    void updTo(QDate);
    void updValid(bool);
    void updOwnerId(int);
    void updResidents(QList<int> &);
    void update();
};

#endif // CONTRACT_H
