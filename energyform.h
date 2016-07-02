#ifndef ENERGYFORM_H
#define ENERGYFORM_H

#include <QDialog>

namespace Ui {
class energyForm;
}

class energyForm : public QDialog
{
    Q_OBJECT

public:
    explicit energyForm(QWidget *parent = 0);
    ~energyForm();

private:
    Ui::energyForm *ui;
};

#endif // ENERGYFORM_H
