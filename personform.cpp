#include "personform.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QPushButton>
#include <QTextEdit>
#include "person.h"

void personForm::createForm()
{
    // Form
    lblName = new QLabel(tr("Name"),this);
    edtName = new QLineEdit(this);
    lblSurname = new QLabel(tr("Surname"),this);
    edtSurname = new QLineEdit(this);
    lblBirthDate = new QLabel(tr("Birth Date"),this);
    edtBirthDate = new QDateEdit(this);
    edtBirthDate->setCalendarPopup(true);
    lblAddress = new QLabel(tr("Address"));
    edtAddress = new QTextEdit(this);
    edtAddress->setMinimumHeight(75);
    lblEmail = new QLabel(tr("Email"),this);
    edtEmail = new QLineEdit(this);
    lblPhone = new QLabel(tr("Phone"),this);
    edtPhone = new QLineEdit(this);
    lblBank = new QLabel(tr("Bank"));
    edtBank = new QLineEdit(this);

    btnSubmit = new QPushButton(tr("Cre&ate"),this);
    btnCancel = new QPushButton(tr("&Cancel"),this);

    QHBoxLayout *lButtons = new QHBoxLayout;
    lButtons->addWidget(btnSubmit);
    lButtons->addWidget(btnCancel);

    QFormLayout *lForm = new QFormLayout;
    lForm->addRow(lblName,edtName);
    lForm->addRow(lblSurname,edtSurname);
    lForm->addRow(lblBirthDate,edtBirthDate);
    lForm->addRow(lblAddress,edtAddress);
    lForm->addRow(lblEmail,edtEmail);
    lForm->addRow(lblPhone,edtPhone);
    lForm->addRow(lblBank,edtBank);
    lForm->setFieldGrowthPolicy(lForm->ExpandingFieldsGrow);

    QVBoxLayout *lMain = new QVBoxLayout;
    lMain->addLayout(lForm);
    lMain->addStretch();
    lMain->addLayout(lButtons);

    setLayout(lMain);

    // Signals & slots
    connect(btnSubmit,SIGNAL(clicked(bool)),this,SLOT(on_submit()));
    connect(btnCancel,SIGNAL(clicked(bool)),this,SLOT(reject()));
}

void personForm::fillForm()
{
    edtName->setText(_person->getName());
    edtSurname->setText(_person->getSurname());
    edtBirthDate->setDate(_person->getBirthDate());
    edtAddress->setText(_person->getAddress());
    edtEmail->setText(_person->getEmail());
    edtPhone->setText(_person->getPhone());
    edtBank->setText(_person->getBank());
}

personForm::personForm(sqlI *_db, Person *p, QWidget *parent)
    : QDialog(parent), addingNew(false), _person(p), ownPerson(true), db(_db)
{
    createForm();
    fillForm();
    setWindowTitle(tr("Edit person"));
    btnSubmit->setText(tr("&Save"));
}

personForm::personForm(sqlI *_db, QWidget *parent)
    : QDialog(parent), addingNew(true), ownPerson(false), db(_db)
{
    createForm();
    setWindowTitle(tr("Create person"));
}

personForm::~personForm()
{
    if(ownPerson) delete _person;
}

void personForm::on_submit()
{
    if(addingNew){
        _person = Person::createPerson(
                    db,
                    edtName->text(),
                    edtSurname->text(),
                    edtBirthDate->date(),
                    edtAddress->toPlainText(),
                    edtEmail->text(),
                    edtPhone->text(),
                    edtBank->text());
        ownPerson=true;
    }else{
        _person->updName(edtName->text());
        _person->updSurname(edtSurname->text());
        _person->updBirthDate(edtBirthDate->date());
        _person->updAddress(edtAddress->toPlainText());
        _person->updEmail(edtEmail->text());
        _person->updPhone(edtPhone->text());
        _person->updBank(edtBank->text());

        _person->update();    
    }
    accept();
}
