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
#include "contract.h"
#include "personform.h"
#include "sqlI.h"

class myListWidget : public QListWidget{
public:
    explicit myListWidget(QWidget *parent = 0) : QListWidget(parent) {}
protected:
    void dragEnterEvent(QDragEnterEvent * event){
        const QMimeData * mime = event->mimeData();
        if(mime->hasText() /*&& mime->hasFormat("application/x-person")*/)
            event->setAccepted(true);
    }

    void dropEvent(QDropEvent * event){
        qDebug() << "Drop";
        const QMimeData *mime = event->mimeData();
        if(mime->hasText() /*&& mime->hasFormat("application/x-person")*/){
            this->addItem(mime->text());
        }
    }
};

contractForm::contractForm(sqlI *_db,contract *c, QWidget *parent)
    : QDialog(parent),addingNew(false),db(_db),_contract(c)
{
    createForm();
    setWindowTitle(tr("Edit contract"));
}

contractForm::contractForm(sqlI *_db,QWidget *parent)
    : QDialog(parent),addingNew(true),db(_db)
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
    /*lblTendants = new QLabel(tr("Tendants"),this);
    lstTendants = new myListWidget(this);
    lstTendants->setAcceptDrops(true);
    lstResidents = new myListWidget(this);
    lstResidents->setAcceptDrops(true);*/
    lblResidents = new QLabel(tr("Residents"),this);
    tblResidents = new QTableView(this);
    //tblResidents->setMinimumHeight(100);
    cmbAddResident = new QComboBox(this);
    tbnAddResident = new QToolButton(this);
    tbnAddResident->setText("+");
    tbnOutResident = new QToolButton(this);
    tbnOutResident->setText("–");
    tbnResidentTendant = new QToolButton(this);
    tbnResidentTendant->setText("RT");

    btnSubmit = new QPushButton(tr("Cre&ate"),this);
    btnCancel = new QPushButton(tr("&Cancel"),this);

    //btnNewResident = new QPushButton(tr("Create a new person"),this);

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
    lForm->addRow(lblMainOwner,cmbMainOwner);
    lForm->addRow(lblResidents);
    lForm->addRow(tblResidents);
    QHBoxLayout *lEdtResidents = new QHBoxLayout;
    lEdtResidents->addWidget(cmbAddResident);
    lEdtResidents->addWidget(tbnAddResident);
    lEdtResidents->addWidget(tbnOutResident);
    lEdtResidents->addWidget(tbnResidentTendant);
    lForm->addRow(lEdtResidents);

    QVBoxLayout *lMain = new QVBoxLayout;
    lMain->addLayout(lForm);
    lMain->addStretch();
    lMain->addLayout(lButtons);

    setLayout(lMain);

    fillPersons();

    // Signals & slots
    connect(cmbMainOwner,SIGNAL(activated(int)),this,SLOT(on_cmbMainOwner_activated(int)));
    //connect(tbnAddResident,SIGNAL(pressed()),this,SLOT(on_tbnAddResident_pressed()));
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

void contractForm::fillPersons()
{
    // Fill Owner ComboBox
    int ownId=0;
    //if(!addingNew) ownId = _contract->getOwnerId();
    QMapIterator<int,QString> person(db->getPersonsName());
    while(person.hasNext()){
        person.next();
        if(!addingNew && ownId == person.key())
            cmbMainOwner->insertItem(0,person.value(),person.key());
        else
            cmbMainOwner->addItem(person.value(),person.key());
            cmbAddResident->addItem(person.value(),person.key());
    }
    cmbMainOwner->insertSeparator(cmbMainOwner->count());
    cmbMainOwner->addItem(tr("Create a new person"));
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

/*void contractForm::on_tbnAddResident_pressed(){
    QDrag *drag = new QDrag(this);
    QMimeData *mime = new QMimeData;
    mime->setText(cmbAddResident->currentText());
    //mime->setProperty("rowid",cmbAddResident->currentData());
    mime->setImageData(cmbAddResident->currentData());
    mime->setData("application/x-person",QByteArray());
    drag->setMimeData(mime);
    drag->exec(Qt::CopyAction | Qt::MoveAction | Qt::LinkAction);
}*/
