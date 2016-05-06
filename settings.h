#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

class QSettings;

class settings : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(QString dbFile READ dbFile WRITE setDbFile)
    //Q_PROPERTY(QString dbType READ dbType WRITE setDbType NOTIFY dbTypeChanged)

    QSettings s;
    QString _dbFile;
    QString _dbType;

public:
    explicit settings(QObject *parent = 0);
    //~settings(); // zbytecne, ~QSettings to ulozi

    void setDbFile(QString _s) {_dbFile=_s;s.setValue("database/file",_s);}
    void dbFileDialog(QWidget *);
    QString dbFile() const {return _dbFile;}
    void setDbType(QString _s) {_dbType=_s;s.setValue("database/type",_s);}
    QString dbType() const {return _dbType;}

signals:

public slots:
};

#endif // SETTINGS_H
