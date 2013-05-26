/****************************************************
ewizard.h

   Part of Data Import for Ontario Tire Stewardship
        Copyright (C) Joshua Netterfield 2012

                 All rights reserved.
*****************************************************/

#ifndef DEMOWIZARD_H
#define DEMOWIZARD_H

#include <QWizard>
#include "ebrowser.h"
#include "eaction.h"
#include "edifreader.h"

namespace Ui {
    class DemoWizard;
}

class EWizard : public QWizard
{
    Q_OBJECT

    EBrowser s_browser;
    EDifReader* s_dif;
    EAction* s_action;
    QStringList s_keys;
public:
    explicit EWizard(QWidget *parent = 0);
    ~EWizard();

protected:
    void closeEvent(QCloseEvent *);
    void reject();

public slots:
    void validatePage0();
    void showEula();
    void autoFillPassword();
    void browseForDif();
    void pageChangeLogic(int p);

    void loginDone();
    void haulerValidationDone();
    void userClaimSelectionDone();
    void difActionDone();

    void error(QString error);
    void collLoginActionDone();
    void info(QString info);
    void showHelp();

private:
    Ui::DemoWizard *ui;
};

#endif // DEMOWIZARD_H
