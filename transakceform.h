#ifndef TRANSAKCEFORM_H
#define TRANSAKCEFORM_H

#include <QDialog>


class sqlI;
class Transakce;
class QAbstractButton;

namespace Ui {
class TransakceForm;
}

class TransakceForm : public QDialog
{
    Q_OBJECT

    sqlI *db;
    Transakce *transakce;
    bool addingNew;
    bool ownTransakce;

    void fillForm();
    void addRow();
    void addRow(QString _konto, int _amount, QString _notice);
    void removeRow();

    void save();

public:
    explicit TransakceForm(sqlI *_db,QWidget *parent = 0);
    explicit TransakceForm(sqlI *_db,Transakce *t,QWidget *parent = 0);
    ~TransakceForm();

private slots:
    void on_btnAdd_clicked();
    bool checkAmount();
    void on_btnRemove_clicked();
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::TransakceForm *ui;
};

#endif // TRANSAKCEFORM_H
