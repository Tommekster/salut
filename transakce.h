#ifndef TRANSAKCE_H
#define TRANSAKCE_H

#include <QDialog>

namespace Ui {
class Transakce;
}

class Transakce : public QDialog
{
    Q_OBJECT

public:
    explicit Transakce(QWidget *parent = 0);
    ~Transakce();

private:
    Ui::Transakce *ui;
};

#endif // TRANSAKCE_H
