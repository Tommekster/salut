#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settings.h"
class sqlI;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QString dbFile;
    sqlI *db;
    settings *s;
    void loadConfiguration();
    void dbConnect();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void loadContracts();
    void loadPersons();
    void loadEnergy();
    void testFn();

    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_tabWidget_currentChanged(int index);

    void on_btnAddContract_clicked();

    void on_btnAddPerson_clicked();

    void on_tableView_2_doubleClicked(const QModelIndex &index);

    void on_comboBox_2_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
