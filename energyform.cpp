#include "energyform.h"
#include "ui_energyform.h"

energyForm::energyForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::energyForm)
{
    ui->setupUi(this);
}

energyForm::~energyForm()
{
    delete ui;
}
