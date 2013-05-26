#ifndef EHELPDIALOG_H
#define EHELPDIALOG_H

#include <QDialog>

namespace Ui {
    class EHelpDialog;
}

class EHelpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EHelpDialog(QWidget *parent = 0);
    ~EHelpDialog();

private:
    Ui::EHelpDialog *ui;
};

#endif // EHELPDIALOG_H
