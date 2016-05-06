#include "contractform.h"

#include <QDateEdit>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QListWidget>
#include <QRadioButton>
#include <QDebug>
#include "contract.h"
#include "personform.h"

contractForm::contractForm(contract *c, QWidget *parent)
    : QDialog(parent),addingNew(false),_contract(c)
{
    createForm();
    setWindowTitle(tr("Edit contract"));
}

contractForm::contractForm(QWidget *parent)
    : QDialog(parent),addingNew(true)
{
    createForm();
    setWindowTitle(tr("Add contract"));
}

void contractForm::createForm()
{
    QSqlQuery q(QSqlDatabase::database("in_mem_db"));
    QDate d = QDate::currentDate();

    // Form
    lblCode = new QLabel(tr("Number of contract"),this);
    edtCode = new QLineEdit(d.toString("yyyy/MM"),this);
    lblValidFrom = new QLabel(tr("Valid from"),this);
    edtValidFrom = new QDateEdit(QDate::currentDate(),this);
    lblValidTo = new QLabel(tr("Valid to"),this);
    edtValidTo = new QDateEdit(QDate::currentDate(),this);
    chkIsValid = new QCheckBox(tr("Contract is active"),this);
    chkIsValid->setChecked(true);
    lblMainOwner = new QLabel(tr("Tendant"),this);
    cmbMainOwner = new QComboBox(this);
    lblResidents = new QLabel(tr("Residents"),this);
    lstResidents = new QListWidget(this);

    btnSubmit = new QPushButton(tr("Cre&ate"),this);
    btnCancel = new QPushButton(tr("&Cancel"),this);

    //btnNewResident = new QPushButton(tr("Create a new person"),this);
    cmbAddResident = new QComboBox(this);

    QHBoxLayout *lButtons = new QHBoxLayout;
    lButtons->addWidget(btnSubmit);
    lButtons->addWidget(btnCancel);

    QVBoxLayout *lMain = new QVBoxLayout;
    lMain->addWidget(lblCode);
    lMain->addWidget(edtCode);
    lMain->addWidget(lblValidFrom);
    lMain->addWidget(edtValidFrom);
    lMain->addWidget(lblValidTo);
    lMain->addWidget(edtValidTo);
    lMain->addWidget(chkIsValid);
    lMain->addWidget(lblMainOwner);
    lMain->addWidget(cmbMainOwner);
    lMain->addWidget(lblResidents);
    lMain->addWidget(lstResidents);
    lMain->addStretch();
    lMain->addLayout(lButtons);

    setLayout(lMain);

    // Fill Owner ComboBox
    q.exec("select rowid, Name || ' ' || Surname from Persons");
    int ownId=0;
    //if(!addingNew) ownId = _contract->getOwnerId();
    while(q.next()){
        int rowid = q.value(0).toInt();
        QString name = q.value(1).toString();
        if(!addingNew && ownId == rowid)
            cmbMainOwner->insertItem(0,name,rowid);
        else
            cmbMainOwner->addItem(name,rowid);
    }
    cmbMainOwner->insertSeparator(cmbMainOwner->count());
    cmbMainOwner->addItem(tr("Create a new person"));

    // Fill Resident ComboBox
    if(addingNew)
        q.exec("select rowid, Name || ' ' || Surname from Persons");
    else
        q.exec("select Persons.rowid, Persons.Name || ' ' || Persons.Surname from Persons "
               "inner join ContractsPersons on ContractsPersons.Person == Persons.rowid "
               "where ContractsPersons.Contract == 1" /*+ contract->getRowid()*/);
    while(q.next()){
        int rowid = q.value(0).toInt();
        QString name = q.value(1).toString();
        cmbAddResident->addItem(name,rowid);
    }
    cmbAddResident->insertSeparator(cmbAddResident->count());
    cmbAddResident->addItem(tr("Create a new person"));

    // Fill Residents ListWidget
    //if(!addingNew){}
    /*QListWidgetItem *lwiNewHabitant = new QListWidgetItem;
    lstResidents->addItem(lwiNewHabitant);
    lstResidents->setItemWidget(lwiNewHabitant,btnNewResident);*/
    QListWidgetItem *lwiAddHabitant = new QListWidgetItem;
    lstResidents->addItem(lwiAddHabitant);
    lstResidents->setItemWidget(lwiAddHabitant,cmbAddResident);

    // Signals & slots
    connect(cmbMainOwner,SIGNAL(activated(int)),this,SLOT(on_cmbMainOwner_activated(int)));
    connect(btnCancel,SIGNAL(clicked(bool)),this,SLOT(reject()));
}

void contractForm::createNewOwner()
{
    personForm form(this);
    form.exec();
}

void contractForm::createNewResident()
{
    personForm form(this);
    form.exec();
}

void contractForm::on_cmbMainOwner_activated(int index)
{
    //qDebug() << "onCmbMainOwnerActivated" << index << ","<<cmbMainOwner->count();
    if((index+1) == cmbMainOwner->count() && !QString::compare(cmbMainOwner->itemText(index),tr("Create a new person"))){
        createNewOwner();
    }
}

void contractForm::on_cmbAddResident_activated(int index)
{
    if((index+1) == cmbAddResident->count() && !QString::compare(cmbAddResident->itemText(index),tr("Create a new person"))){
        createNewResident();
    //}else{

    }
}
