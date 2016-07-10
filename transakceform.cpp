#include "transakceform.h"
#include "ui_transakceform.h"
#include "transakce.h"
#include "sqlI.h"

#include <QComboBox>
#include <QMessageBox>

void TransakceForm::fillForm()
{
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnWidth(0,150);
    ui->tableWidget->setColumnWidth(1,100);
    ui->tableWidget->setColumnWidth(2,250);
    QStringList labels; labels << tr("Konto") << tr("Částka") << tr("Poznámka");
    ui->tableWidget->setHorizontalHeaderLabels(labels);

    connect(ui->spnAmount,SIGNAL(valueChanged(int)),this,SLOT(checkAmount()));

    if(addingNew){
        ui->dateEdit->setDate(QDate::currentDate());
    } else {
        ui->dateEdit->setDate(transakce->getDatum());
        ui->spnAmount->setValue(transakce->getSum());

        QList<Transakce::Rozpis> r=transakce->getRozpis();
        QList<Transakce::Rozpis>::const_iterator i;
        for(i=r.constBegin(); i != r.constEnd(); ++i){
            addRow((*i).konto,(*i).amount,(*i).notice);
        }
    }
    checkAmount();
}

void TransakceForm::addRow()
{
    addRow(QString(),0,QString(""));
}

void TransakceForm::addRow(QString _konto,int _amount, QString _notice)
{
    QComboBox *konto=new QComboBox(this);
    QStringList lstKonta=db->getTransakceAccounts();
    //lstKonta << "Jedno konto" << "druhe konto" << "dalsi konto";
    konto->addItems(lstKonta);
    konto->setEditable(true);
    if(!_konto.isNull()) //konto->addItem(_konto);
        konto->setCurrentText(_konto);

    QSpinBox *amount=new QSpinBox(this);
    amount->setSuffix(tr(" Kč"));
    amount->setMinimum(-999999);
    amount->setMaximum(999999);
    amount->setValue(_amount);
    connect(amount,SIGNAL(valueChanged(int)),this,SLOT(checkAmount()));

    QComboBox *notice=new QComboBox(this);
    QStringList lstNotice=db->getTransakceNotices();
    //lstNotice << "Nájem" << "Zálohy" << "Jistota";
    notice->addItems(lstNotice);
    notice->setEditable(true);
    notice->setCurrentText(_notice);

    int newLine=ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(newLine);
    ui->tableWidget->setCellWidget(newLine,0,konto);
    ui->tableWidget->setCellWidget(newLine,1,amount);
    ui->tableWidget->setCellWidget(newLine,2,notice);
}

void TransakceForm::removeRow()
{
    int toRemove = ui->tableWidget->currentRow();
    ui->tableWidget->removeRow(toRemove);
}

void TransakceForm::save()
{
    if(!checkAmount()){
        QMessageBox::critical(this,tr("Špatně vyplněný formulář!"),tr("Neodpovídá součet transakce. Prosím, opravte hodnoty. "));
        return;
    }

    QList<Transakce::Rozpis> rozpis;
    int rows = ui->tableWidget->rowCount();
    for(int i=0; i<rows;i++){
        QComboBox *konto  = (QComboBox *) ui->tableWidget->cellWidget(i,0);
        QSpinBox  *amount =  (QSpinBox *) ui->tableWidget->cellWidget(i,1);
        QComboBox *notice = (QComboBox *) ui->tableWidget->cellWidget(i,2);

        rozpis << Transakce::Rozpis(konto->currentText(),
                                    amount->value(),
                                    notice->currentText());
    }

    if(addingNew){
        transakce = Transakce::createTransakce(db,
                                               ui->dateEdit->date(),
                                               ui->spnAmount->value(),
                                               rozpis);
        ownTransakce = true;
    }else{
        transakce->updDatum(ui->dateEdit->date());
        transakce->updSum(ui->spnAmount->value());
        transakce->updRozpis(rozpis);

        transakce->update();
    }
}

TransakceForm::TransakceForm(sqlI *_db, QWidget *parent) :
    db(_db),
    QDialog(parent),
    ui(new Ui::TransakceForm),addingNew(true),ownTransakce(false)
{
    ui->setupUi(this);
    setWindowTitle(tr("Add transaction"));
    ui->buttonBox->setStandardButtons(QDialogButtonBox::Cancel);
    ui->buttonBox->addButton(tr("Insert"),QDialogButtonBox::AcceptRole);
    fillForm();
}

TransakceForm::TransakceForm(sqlI *_db, Transakce *t, QWidget *parent) :
    db(_db),transakce(t),
    QDialog(parent),
    ui(new Ui::TransakceForm),addingNew(false),ownTransakce(true)
{
    ui->setupUi(this);
    setWindowTitle(tr("Edit transaction"));
    ui->buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Save);
    fillForm();
}

TransakceForm::~TransakceForm()
{
    delete ui;
    if(ownTransakce) delete transakce;
}

void TransakceForm::on_btnAdd_clicked()
{
    addRow();
    checkAmount();
}

bool TransakceForm::checkAmount()
{
    int rows = ui->tableWidget->rowCount();
    int sum = 0;
    for(int i=0; i<rows;i++){
        QSpinBox *spin = (QSpinBox *)ui->tableWidget->cellWidget(i,1);
        sum += spin->value();
    }

    if(sum == ui->spnAmount->value()){
        ui->spnAmount->setStyleSheet("");
        return true;
    }else{
        ui->spnAmount->setStyleSheet("background: #f88;");
        return false;
    }
}

void TransakceForm::on_btnRemove_clicked()
{
    removeRow();
    checkAmount();
}

void TransakceForm::on_buttonBox_accepted()
{
    save();
}
