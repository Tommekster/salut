#ifndef TRANSAKCEFORM_H
#define TRANSAKCEFORM_H

#include <QDialog>

namespace Ui {
class TransakceForm;
}

class TransakceForm : public QDialog
{
    Q_OBJECT

    void addRow();
    void addRow(QString _konto, int _amount, QString _notice);
    void removeRow();

public:
    explicit TransakceForm(QWidget *parent = 0);
    ~TransakceForm();

private slots:
    void on_btnAdd_clicked();
    void checkAmount();

    void on_btnRemove_clicked();

private:
    Ui::TransakceForm *ui;
};

#endif // TRANSAKCEFORM_H
