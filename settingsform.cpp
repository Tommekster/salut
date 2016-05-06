#include "settingsform.h"

#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QSettings>

settingsForm::settingsForm(QWidget *parent) : QWidget(parent)
{
    QSettings s("config.ini",QSettings::IniFormat);
    edtDatabaseType = new QTextEdit(s.value("").toString(),this);
}
