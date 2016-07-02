#ifndef ENERGYFORM_H
#define ENERGYFORM_H

#include <QDialog>

class sqlI;
class EnergyRecord;

namespace Ui {
class energyForm;
}

class energyForm : public QDialog
{
    Q_OBJECT

    sqlI *db;
    EnergyRecord *record;
    bool addingNew;
    bool ownRecord;

    void fillForm();

public:
    explicit energyForm(sqlI *d,QWidget *parent = 0);
    explicit energyForm(sqlI *d,EnergyRecord *r,QWidget *parent = 0);

    ~energyForm();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::energyForm *ui;
};

#endif // ENERGYFORM_H
