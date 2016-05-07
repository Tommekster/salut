#include "personform.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>

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
    connect(btnCancel,SIGNAL(clicked(bool)),this,SLOT(reject()));
}

personForm::personForm(person *p, QDialog *parent)
    : QDialog(parent), addingNew(false)
{
    createForm();
    setWindowTitle(tr("Edit person"));
}

personForm::personForm(QDialog *parent)
    : QDialog(parent), addingNew(true)
{
    createForm();
    setWindowTitle(tr("Create person"));
}
