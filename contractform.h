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
class QTableView;
class QToolButton;
class sqlI;
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
    QComboBox *cmbAddResident;
    QToolButton *tbnAddResident;
    QToolButton *tbnOutResident;

    QPushButton *btnSubmit;
    QPushButton *btnCancel;

    void createForm();
    void createNewPerson();
    void fillPersons();
    void fillResidents();
    bool addingNew;
    sqlI *db;
    contract *_contract;

protected slots:
    void on_cmbMainOwner_activated(int);
    void on_cmbAddResident_activated(int);
    void on_tbnAddResident_clicked();
    void on_tbnOutResident_clicked();

public:
    explicit contractForm(sqlI *_db,QWidget *parent = 0);
    explicit contractForm(sqlI *_db,contract *c, QWidget *parent = 0);

signals:

public slots:
protected slots:
};

#endif // CONTRACTFORM_H
