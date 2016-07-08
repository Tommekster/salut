#include "transakceform.h"
#include "ui_transakceform.h"

#include <QComboBox>

void TransakceForm::fillForm()
{
    ui->dateEdit->setDate(QDate::currentDate());

    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnWidth(0,150);
    ui->tableWidget->setColumnWidth(1,100);
    ui->tableWidget->setColumnWidth(2,250);
    QStringList labels; labels << tr("Konto") << tr("Částka") << tr("Poznámka");
    ui->tableWidget->setHorizontalHeaderLabels(labels);

    connect(ui->spnAmount,SIGNAL(valueChanged(int)),this,SLOT(checkAmount()));
}

void TransakceForm::addRow()
{
    addRow(QString(),0,QString(""));
}

void TransakceForm::addRow(QString _konto,int _amount, QString _notice)
{
    QComboBox *konto=new QComboBox(this);
    QStringList lstKonta; lstKonta << "Jedno konto" << "druhe konto" << "dalsi konto";
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
    QStringList lstNotice; lstNotice << "Nájem" << "Zálohy" << "Jistota";
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

}

TransakceForm::TransakceForm(sqlI *_db, QWidget *parent) :
    db(_db),
    QDialog(parent),
    ui(new Ui::TransakceForm),addingNew(true),ownTransakce(false)
{
    ui->setupUi(this);

    fillForm();
}

TransakceForm::TransakceForm(sqlI *_db, Transakce *t, QWidget *parent) :
    db(_db),transakce(t),
    QDialog(parent),
    ui(new Ui::TransakceForm),addingNew(false),ownTransakce(true)
{
    ui->setupUi(this);

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

void TransakceForm::checkAmount()
{
    int rows = ui->tableWidget->rowCount();
    int sum = 0;
    for(int i=0; i<rows;i++){
        QSpinBox *spin = (QSpinBox *)ui->tableWidget->cellWidget(i,1);
        sum += spin->value();
    }

    if(sum == ui->spnAmount->value()){
        ui->spnAmount->setStyleSheet("");
    }else{
        ui->spnAmount->setStyleSheet("background: #f88;");
    }
}

void TransakceForm::on_btnRemove_clicked()
{
    removeRow();
    checkAmount();
}
