#ifndef PERSONFORM_H
#define PERSONFORM_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QTextEdit;
class QPushButton;
class Person;
class sqlI;

class personForm : public QDialog
{
    Q_OBJECT

    QLabel *lblName;
    QLineEdit *edtName;
    QLabel *lblSurname;
    QLineEdit *edtSurname;
    QLabel *lblAddress;
    QTextEdit *edtAddress;
    QLabel *lblEmail;
    QLineEdit *edtEmail;
    QLabel *lblPhone;
    QLineEdit *edtPhone;
    QLabel *lblBank;
    QLineEdit *edtBank;

    QPushButton *btnSubmit;
    QPushButton *btnCancel;

    void createForm();
    void fillForm();

    bool addingNew;
    Person *_person;
    bool ownPerson;
    sqlI *db;

public:
    explicit personForm(sqlI *_db, Person *p, QWidget *parent = 0);
    explicit personForm(sqlI *_db, QWidget *parent = 0);
    ~personForm();

    Person *getPerson(){ownPerson=false;return _person;}

signals:

protected slots:
    void on_submit();

public slots:
};

#endif // PERSONFORM_H
