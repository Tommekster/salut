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
        int rowid;
        QString konto;
        int amount;
        QString notice;
        Rozpis(QString k, int a, QString n):rowid(-1),konto(k),amount(a),notice(n){}
        Rozpis(int id,QString k, int a, QString n):rowid(id),konto(k),amount(a),notice(n){}
        bool operator==(const Rozpis &rhs) const{
            return (amount == rhs.amount) && (konto.compare(rhs.konto) == 0) && (notice.compare(rhs.notice) == 0);
        }
    };

private:
    sqlI *db;

    int rowid;
    QDate datum;
    int sum;
    QList<Rozpis> rozpis;

    bool isNew;
    void save();
    bool chDatum,chSum;
    QList<Rozpis> newRozpis;
    QList<int> removeRozpis;

public:
    Transakce(sqlI *_db);
    Transakce(sqlI *_db,int);

    static Transakce *createTransakce(sqlI *db,QDate datum,int sum,const QList<Rozpis> &rozpis);

    int getRowId()const{return rowid;}
    QDate getDatum()const{return datum;}
    int getSum()const{return sum;}
    QList<Rozpis> getRozpis()const{return rozpis;}
    QList<Rozpis> *getNewRozpis(){return &newRozpis;}

    void setDatum(QDate d){datum=d;}
    void setSum(int i){sum=i;}
    void appendRozpis(const QList<Rozpis> &l){rozpis.append(l);}
    void appendRozpis(Rozpis r){rozpis.append(r);}

    void updDatum(QDate);
    void updSum(int);
    void updRozpis(QList<Rozpis>);
    void update();
};

#endif // TRANSAKCE_H
