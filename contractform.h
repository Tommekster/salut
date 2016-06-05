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
class Contract;

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
    void fillForm();
    void createNewPerson();
    void fillPersons();
    void fillResidents();
    bool addingNew;
    sqlI *db;
    Contract *_contract;
    bool ownContract;

protected slots:
    void on_cmbMainOwner_activated(int);
    void on_cmbAddResident_activated(int);
    void on_tbnAddResident_clicked();
    void on_tbnOutResident_clicked();
    void on_submit();

public:
    explicit contractForm(sqlI *_db,QWidget *parent = 0);
    explicit contractForm(sqlI *_db,Contract *c, QWidget *parent = 0);
    ~contractForm();

signals:

public slots:
protected slots:
};

#endif // CONTRACTFORM_H
