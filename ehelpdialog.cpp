#include "ehelpdialog.h"
#include "ui_ehelpdialog.h"

EHelpDialog::EHelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EHelpDialog)
{
    ui->setupUi(this);
}

EHelpDialog::~EHelpDialog()
{
    delete ui;
}
