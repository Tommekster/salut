#include "personform.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
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
    lblAddress = new QLabel(tr("Address"));
    edtAddress = new QTextEdit(this);
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
    lForm->addRow(lblAddress,edtAddress);
    lForm->addRow(lblEmail,edtEmail);
    lForm->addRow(lblPhone,edtPhone);
    lForm->addRow(lblBank,edtBank);

    QVBoxLayout *lMain = new QVBoxLayout;
    lMain->addLayout(lForm);
    lMain->addStretch();
    lMain->addLayout(lButtons);

    setLayout(lMain);

    // Signals & slots
    connect(btnSubmit,SIGNAL(clicked(bool)),this,SLOT(on_submit()));
    connect(btnCancel,SIGNAL(clicked(bool)),this,SLOT(reject()));
}

personForm::personForm(sqlI *_db, Person *p, QDialog *parent)
    : QDialog(parent), addingNew(false), db(_db), _person(p), ownPerson(false)
{
    createForm();
    setWindowTitle(tr("Edit person"));
    btnSubmit->setText(tr("&Save"));
}

personForm::personForm(sqlI *_db, QDialog *parent)
    : QDialog(parent), addingNew(true), db(_db)
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
                    edtAddress->toPlainText(),
                    edtEmail->text(),
                    edtPhone->text(),
                    edtBank->text());
        ownPerson=true;
        accept();
    }
}
