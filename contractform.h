#ifndef CONTRACTFORM_H
#define CONTRACTFORM_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QDateEdit;
class QPushButton;
class QCheckBox;
class QComboBox;
class QListWidget;
class contract;

class contractForm : public QDialog
{
    Q_OBJECT

    QLabel *lblCode;
    QLineEdit *edtCode;
    QLabel *lblValidFrom;
    QDateEdit *edtValidFrom;
    QLabel *lblValidTo;
    QDateEdit *edtValidTo;
    QCheckBox *chkIsValid;
    QLabel *lblMainOwner;
    QComboBox *cmbMainOwner;
    QLabel *lblResidents;
    QListWidget *lstResidents;

    QPushButton *btnSubmit;
    QPushButton *btnCancel;

    //QPushButton *btnNewResident;
    //QComboBox *cmbCpyHabitants;
    QComboBox *cmbAddResident;

    void createForm();
    void createNewOwner();
    void createNewResident();
    bool addingNew;
    contract *_contract;

private slots:
    void on_cmbMainOwner_activated(int);
    void on_cmbAddResident_activated(int);

public:
    explicit contractForm(QWidget *parent = 0);
    explicit contractForm(contract *c, QWidget *parent = 0);

signals:

public slots:
};

#endif // CONTRACTFORM_H
