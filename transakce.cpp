#include "transakce.h"
#include "ui_transakce.h"

Transakce::Transakce(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Transakce)
{
    ui->setupUi(this);
}

Transakce::~Transakce()
{
    delete ui;
}
