#include "energyform.h"
#include "ui_energyform.h"

#include "sqlI.h"
#include "energyrecord.h"

void energyForm::fillForm()
{
    if(addingNew){
        ui->dateEdit->setDate(QDate::currentDate());
    }else{
        ui->dateEdit->setDate(record->getDatum());
    }

    QList<unsigned int> hydrometers = record->getHydrometers();
    //if(hydrometers.length()<6) return -2;
    ui->wm0->setValue(hydrometers.at(0));
    ui->wm1->setValue(hydrometers.at(1));
    ui->wm2->setValue(hydrometers.at(2));
    ui->wm3->setValue(hydrometers.at(3));
    ui->wm4->setValue(hydrometers.at(4));
    ui->wms->setValue(hydrometers.at(5));

    QList<unsigned int> calorimeters = record->getCalorimeters();
    //if(calorimeters.length()<4) return -3;
    ui->gas->setValue(record->getGasometerValue());
    ui->cm1->setValue(calorimeters.at(0));
    ui->cm2->setValue(calorimeters.at(1));
    ui->cm3->setValue(calorimeters.at(2));
    ui->cm4->setValue(calorimeters.at(3));

    QList<EnergyRecord::EletricityMeter> eletricity_meters = record->getEletricityMeters();
    //if(eletricity_meters.length()<4) return -4;
    ui->vt1->setValue(eletricity_meters.at(0).vt);
    ui->nt1->setValue(eletricity_meters.at(0).nt);
    ui->vt2->setValue(eletricity_meters.at(1).vt);
    ui->nt2->setValue(eletricity_meters.at(1).nt);
    ui->vt3->setValue(eletricity_meters.at(2).vt);
    ui->nt3->setValue(eletricity_meters.at(2).nt);
    ui->vt4->setValue(eletricity_meters.at(3).vt);
    ui->nt4->setValue(eletricity_meters.at(3).nt);

    if(addingNew){
        delete record;
    }
}

energyForm::energyForm(sqlI *d,QWidget *parent) :
    QDialog(parent),db(d),
    addingNew(true),ownRecord(false),
    ui(new Ui::energyForm)
{
    record=EnergyRecord::lastEnergyRecord(db);
    ui->setupUi(this);
    setWindowTitle(tr("Add energy record"));
    ui->buttonBox->setStandardButtons(QDialogButtonBox::Cancel);
    ui->buttonBox->addButton(tr("Create"),QDialogButtonBox::AcceptRole);
    fillForm();
}

energyForm::energyForm(sqlI *d, EnergyRecord *r, QWidget *parent) :
    QDialog(parent),db(d),record(r),
    addingNew(false),ownRecord(true),
    ui(new Ui::energyForm)
{
    ui->setupUi(this);
    setWindowTitle(tr("Edit energy record"));
    ui->buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Save);
    fillForm();
}

energyForm::~energyForm()
{
    delete ui;
    if(ownRecord) delete record;
}

void energyForm::on_buttonBox_accepted()
{
    QList<unsigned int> hydrometers;
    hydrometers << ui->wm0->value()
                << ui->wm1->value()
                << ui->wm2->value()
                << ui->wm3->value()
                << ui->wm4->value()
                << ui->wms->value();

    QList<unsigned int> calorimeters;
    calorimeters << ui->cm1->value()
                 << ui->cm1->value()
                 << ui->cm1->value()
                 << ui->cm1->value();

    QList<EnergyRecord::EletricityMeter> eletricity_meters;
    eletricity_meters << EnergyRecord::EletricityMeter(ui->vt1->value(),ui->nt1->value())
                      << EnergyRecord::EletricityMeter(ui->vt2->value(),ui->nt2->value())
                      << EnergyRecord::EletricityMeter(ui->vt3->value(),ui->nt3->value())
                      << EnergyRecord::EletricityMeter(ui->vt4->value(),ui->nt4->value());
    if(addingNew){
        record = EnergyRecord::createEnergyRecord(db,
                                                      ui->dateEdit->date(),
                                                      hydrometers,
                                                      calorimeters,
                                                      ui->gas->value(),
                                                      eletricity_meters);
        ownRecord=true;
    }else{
        record->update();
    }
}
