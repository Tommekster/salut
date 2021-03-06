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
#include <QFormLayout>
#include <QToolButton>
#include <QDrag>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QTableView>
#include <QMessageBox>
#include "contract.h"
#include "personform.h"
#include "sqlI.h"

contractForm::contractForm(sqlI *_db,Contract *c, QWidget *parent)
    : QDialog(parent),addingNew(false),db(_db),_contract(c),ownContract(true)
{
    createForm();
    fillForm();
    setWindowTitle(tr("Edit contract"));
    btnSubmit->setText(tr("&Save"));
}

contractForm::~contractForm()
{
    if(ownContract) delete _contract;
}

contractForm::contractForm(sqlI *_db,QWidget *parent)
    : QDialog(parent),addingNew(true),db(_db),ownContract(false)
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
    edtValidFrom->setCalendarPopup(true);
    lblValidTo = new QLabel(tr("Valid to"),this);
    edtValidTo = new QDateEdit(QDate::currentDate(),this);
    edtValidTo->setCalendarPopup(true);
    chkIsValid = new QCheckBox(tr("Contract is active"),this);
    chkIsValid->setChecked(true);
    lblFlat = new QLabel(tr("Flat"),this);
    cmbFlat = new QComboBox(this);
    lblMainOwner = new QLabel(tr("Tendant"),this);
    cmbMainOwner = new QComboBox(this);
    lblResidents = new QLabel(tr("Residents"),this);
    lstResidents = new QListWidget;
    cmbAddResident = new QComboBox(this);
    tbnAddResident = new QToolButton(this);
    tbnAddResident->setText("+");
    tbnOutResident = new QToolButton(this);
    tbnOutResident->setText("–");

    btnSubmit = new QPushButton(tr("Cre&ate"),this);
    btnCancel = new QPushButton(tr("&Cancel"),this);

    QHBoxLayout *lButtons = new QHBoxLayout;
    lButtons->addStretch();
    lButtons->addWidget(btnSubmit);
    lButtons->addWidget(btnCancel);

    QFormLayout *lForm = new QFormLayout;
    lForm->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    lForm->addRow(lblCode,edtCode);
    lForm->addRow(lblValidFrom,edtValidFrom);
    lForm->addRow(lblValidTo,edtValidTo);
    lForm->addRow("",chkIsValid);
    lForm->addRow(lblFlat,cmbFlat);
    lForm->addRow(lblMainOwner,cmbMainOwner);
    lForm->addRow(lblResidents);
    lForm->addRow(lstResidents);
    QHBoxLayout *lEdtResidents = new QHBoxLayout;
    lEdtResidents->addWidget(cmbAddResident);
    lEdtResidents->addWidget(tbnAddResident);
    lEdtResidents->addWidget(tbnOutResident);
    lForm->addRow(lEdtResidents);

    QVBoxLayout *lMain = new QVBoxLayout;
    lMain->addLayout(lForm);
    lMain->addStretch();
    lMain->addLayout(lButtons);

    setLayout(lMain);

    fillPersons();
    fillFlats();

    // Signals & slots
    connect(cmbMainOwner,SIGNAL(activated(int)),this,SLOT(on_cmbMainOwner_activated(int)));
    connect(cmbAddResident,SIGNAL(activated(int)),this,SLOT(on_cmbAddResident_activated(int)));
    connect(tbnAddResident,SIGNAL(clicked(bool)),this,SLOT(on_tbnAddResident_clicked()));
    connect(tbnOutResident,SIGNAL(clicked(bool)),this,SLOT(on_tbnOutResident_clicked()));
    connect(btnSubmit,SIGNAL(clicked(bool)),this,SLOT(on_submit()));
    connect(btnCancel,SIGNAL(clicked(bool)),this,SLOT(reject()));
}

void contractForm::fillForm()
{
    edtCode->setText(_contract->getCode());
    edtValidFrom->setDate(_contract->getFrom());
    edtValidTo->setDate(_contract->getTo());
    chkIsValid->setChecked(_contract->isValid());
    cmbFlat->setCurrentIndex(cmbFlat->findData(_contract->getFlatId()));
    cmbMainOwner->setCurrentIndex(cmbMainOwner->findData(_contract->getOwnerId()));
    QList<int> residents=_contract->getResidentsIDs();

    QList<int>::const_iterator i;
    for (i = residents.constBegin(); i != residents.constEnd(); ++i){
        cmbAddResident->setCurrentIndex(cmbAddResident->findData(*i));
        // qDebug() << "resident ID: " << *i << ", text: "<<cmbAddResident->currentText();
        on_tbnAddResident_clicked();
    }

}

void contractForm::createNewPerson()
{
    personForm form(db,this);
    int res=form.exec();
    if(res==form.Accepted){
        fillPersons();
    }
}

void contractForm::fillPersons()
{
    if(cmbMainOwner->count()>0) cmbMainOwner->clear();
    if(cmbAddResident->count()>0) cmbAddResident->clear();
    // Fill Owner ComboBox
    QMapIterator<int,QString> person(db->getPersonsName());
    while(person.hasNext()){
        person.next();
        cmbMainOwner->addItem(person.value(),person.key());
        cmbAddResident->addItem(person.value(),person.key());
    }
    cmbMainOwner->insertSeparator(cmbMainOwner->count());
    cmbMainOwner->addItem(tr("Create a new person"));

    cmbAddResident->insertSeparator(cmbAddResident->count());
    cmbAddResident->addItem(tr("Create a new person"));
}

void contractForm::fillFlats()
{
    if(cmbFlat->count()>0) cmbFlat->clear();
    QMapIterator<int,QString> flat(db->getFlatsName());
    while(flat.hasNext()){
        flat.next();
        cmbFlat->addItem(flat.value(),flat.key());
    }
}

void contractForm::on_cmbMainOwner_activated(int index)
{
    //qDebug() << "onCmbMainOwnerActivated" << index << ","<<cmbMainOwner->count();
    if((index+1) == cmbMainOwner->count() && !QString::compare(cmbMainOwner->itemText(index),tr("Create a new person"))){
        createNewPerson();
    }
}

void contractForm::on_cmbAddResident_activated(int index)
{
    if((index+1) == cmbAddResident->count() && !QString::compare(cmbAddResident->itemText(index),tr("Create a new person"))){
        createNewPerson();
    }
}

void contractForm::on_tbnAddResident_clicked()
{
    // Polozka Create a new person
    if((cmbAddResident->currentIndex()+1) == cmbAddResident->count() && !QString::compare(cmbAddResident->currentText(),tr("Create a new person")))
        return;

    // kontrolu na "duplikaty"
    QList<QListWidgetItem *> l = lstResidents->findItems(cmbAddResident->currentText(),Qt::MatchCaseSensitive);
    if(!l.empty()) return;

    QListWidgetItem *itm = new QListWidgetItem(cmbAddResident->currentText());
    itm->setData(Qt::UserRole,cmbAddResident->currentData());
    lstResidents->addItem(itm);
}

void contractForm::on_tbnOutResident_clicked()
{
    if(lstResidents->count() == 0) return;

    lstResidents->takeItem(lstResidents->currentRow());
}

void contractForm::on_submit()
{
    // kontrola spravnosti dat: treba create a new person nemuze byt drzitel sml.
    if((cmbMainOwner->currentIndex()+1) == cmbMainOwner->count() &&
            !QString::compare(cmbMainOwner->currentText(),tr("Create a new person"))){
        QMessageBox::warning(this,tr("Choose contract`s owner!"),tr("Select contract`s owner! "));
        return;
    }

    // vycuc rezidentu ze seznamu
    QList<int> residents;
    for(int i=0;i<lstResidents->count();i++){
        QListWidgetItem *wi=lstResidents->item(i);
        residents << wi->data(Qt::UserRole).toInt();
    }
    if(addingNew){
        _contract = Contract::createContract(
                    db,
                    edtCode->text(),
                    edtValidFrom->date(),
                    edtValidTo->date(),
                    chkIsValid->isChecked(),
                    cmbMainOwner->currentData().toInt(),
                    cmbFlat->currentData().toInt(),
                    residents);
        ownContract = true;
    }else{
        _contract->updCode(edtCode->text());
        _contract->updFrom(edtValidFrom->date());
        _contract->updTo(edtValidTo->date());
        _contract->updValid(chkIsValid->isChecked());
        _contract->updFlatId(cmbFlat->currentData().toInt());
        _contract->updOwnerId(cmbMainOwner->currentData().toInt());
        _contract->updResidents(residents);

        _contract->update();
    }
    accept();
}
