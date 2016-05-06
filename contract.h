#ifndef CONTRACT_H
#define CONTRACT_H

#include <QString>
#include <QDate>
#include <QList>

class person;

class contract
{
    int rowid;
    QString code;
    QDate vFrom;
    QDate vTo;
    bool valid;
    int ownerId;
    person *owner;

public:
    contract();

    // gettry
    int getRowId()const{return rowid;}
    QString getCode()const{return code;}
    QDate getFrom()const{return vFrom;}
    QDate getTo()const{return vTo;}
    bool isValid()const{return valid;}
    person *getOwner()const;
    int getOwnerId()const{return ownerId;}
    QList<person> getOwners();
};

#endif // CONTRACT_H
