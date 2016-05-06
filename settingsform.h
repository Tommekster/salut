#ifndef SETTINGSFORM_H
#define SETTINGSFORM_H

#include <QWidget>

class QTextEdit;
class QLabel;
class QPushButton;

class settingsForm : public QWidget
{
    Q_OBJECT
public:
    explicit settingsForm(QWidget *parent = 0);

    QTextEdit *edtDatabaseType;
    QLabel *lblDatabaseType;
    QPushButton *btnSubmit;
    QPushButton *btnCancel;

signals:

public slots:
};

#endif // SETTINGSFORM_H
