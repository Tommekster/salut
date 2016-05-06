#ifndef PERSONFORM_H
#define PERSONFORM_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QTextEdit;
class QPushButton;
class person;

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
    bool addingNew;

public:
    explicit personForm(person *p,QDialog *parent = 0);
    explicit personForm(QDialog *parent = 0);

signals:

public slots:
};

#endif // PERSONFORM_H
