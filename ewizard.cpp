/****************************************************
ewizard.cpp

   Part of Data Import for Ontario Tire Stewardship
        Copyright (C) Joshua Netterfield 2012

                 All rights reserved.
*****************************************************/

#include "ewizard.h"
#include "ui_demowizard.h"
#include "ewizardpage.h"
#include "eloginaction.h"
#include "ehaulervalidateaction.h"
#include "eselectclaimaction.h"
#include <QFile>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>
#include <QCompleter>
#include <QCloseEvent>
#include "edifaction.h"
#include "eeula.h"
#include "ehelpdialog.h"
#include "enewclaimaction.h"
#include "edeleteclaimsaction.h"
#include "ecolllogininfoaction.h"
#include "ecollaction.h"

EWizard::EWizard(QWidget *parent) :
    QWizard(parent), s_dif(0), s_action(0), ui(new Ui::DemoWizard)
{
    s_browser.hide();
    ui->setupUi(this);
    static_cast<EWizardPage*>(page(0))->setComplete(0);
    QSettings settings("Nettek","Data Import for Ontario Tire Stewardship");
    QStringList keys=settings.value("Keys",QStringList("")).toStringList();
    s_keys=keys;
    for(int i=0;i<keys.size();i++)
    {
        keys[i].truncate(keys.at(i).indexOf(":"));
    }
    ui->lineEdit_username->setCompleter(new QCompleter(keys));
    keys=settings.value("HaulerNums",QStringList("")).toStringList();
    ui->lineEdit_haulerNumber->setCompleter(new QCompleter(keys));
    connect(ui->lineEdit_haulerNumber,SIGNAL(textChanged(QString)),this,SLOT(validatePage0()));
    connect(ui->lineEdit_navyDif,SIGNAL(textChanged(QString)),this,SLOT(validatePage0()));
    connect(ui->lineEdit_password,SIGNAL(textChanged(QString)),this,SLOT(validatePage0()));
    connect(ui->lineEdit_username,SIGNAL(textChanged(QString)),this,SLOT(validatePage0()));
    connect(ui->lineEdit_username,SIGNAL(textChanged(QString)),this,SLOT(autoFillPassword()));
    connect(ui->comboBox_action,SIGNAL(currentIndexChanged(int)),this,SLOT(validatePage0()));
    connect(ui->toolButton_navyDif,SIGNAL(clicked()),this,SLOT(browseForDif()));
    connect(this,SIGNAL(currentIdChanged(int)),this,SLOT(pageChangeLogic(int)));
    connect(ui->checkBox_showWeb,SIGNAL(clicked(bool)),&s_browser,SLOT(setVisible(bool)));
    connect(this,SIGNAL(helpRequested()),this,SLOT(showHelp()));
    connect(&s_browser,SIGNAL(visiblityChanged(bool)),ui->checkBox_showWeb,SLOT(setChecked(bool)));

    connect(ui->lineEdit_invCredit,SIGNAL(textChanged(QString)),this,SLOT(validatePage0()));
    connect(ui->lineEdit_invKg,SIGNAL(textChanged(QString)),this,SLOT(validatePage0()));
    connect(ui->lineEdit_phone,SIGNAL(textChanged(QString)),this,SLOT(validatePage0()));
    connect(ui->lineEdit_preparedBy,SIGNAL(textChanged(QString)),this,SLOT(validatePage0()));
}


EWizard::~EWizard()
{
    delete ui;
}

void EWizard::closeEvent(QCloseEvent *event)
{
    if(QMessageBox::question(this,"Really quit?","Are you sure you want to quit this wizard? Keep in mind that data may have been submitted.",QMessageBox::Yes,QMessageBox::No)==QMessageBox::No)
    {
        event->ignore();
    }
    else
    {
        event->accept();
        qApp->quit();
    }
}

void EWizard::reject()
{
    if(QMessageBox::question(this,"Really quit?","Are you sure you want to quit this wizard? Keep in mind that data may have been submitted.",QMessageBox::Yes,QMessageBox::No)==QMessageBox::No)
    {
        return;
    }
    else
    {
        QDialog::reject();
        qApp->quit();
    }
}

void EWizard::validatePage0()
{
    EWizardPage* page=static_cast<EWizardPage*>(this->page(0));

    if(ui->comboBox_action->currentIndex()==1)
    {
        ui->label_username->show();
        ui->lineEdit_username->show();
        ui->label_password->show();
        ui->lineEdit_password->show();

        ui->label_claimPeriod->hide();
        ui->comboBox_month->hide();
        ui->comboBox_year->hide();

        ui->label_openInvCredit->hide();
        ui->lineEdit_invCredit->hide();

        ui->label_openInvKg->hide();
        ui->lineEdit_invKg->hide();

        ui->label_phone->hide();
        ui->lineEdit_phone->hide();

        ui->label_preparedBy->hide();
        ui->lineEdit_preparedBy->hide();

        ui->label_dif->show();
        ui->lineEdit_navyDif->show();
        ui->toolButton_navyDif->show();
        ui->label_haulerNumber->show();
        ui->lineEdit_haulerNumber->show();
        ui->label_claim_collector->hide();
        ui->comboBox_claim_collector->hide();
        ui->formLayout->setSpacing(-1);
        setFixedHeight(350);
        if(!QFile(ui->lineEdit_navyDif->text()).exists())
        {
            page->setComplete(0);
            return;
        }
    }
    else if(ui->comboBox_action->currentIndex()==2)
    {
        ui->label_username->show();
        ui->lineEdit_username->show();
        ui->label_password->show();
        ui->lineEdit_password->show();

        ui->label_claimPeriod->hide();
        ui->comboBox_month->hide();
        ui->comboBox_year->hide();

        ui->label_openInvCredit->hide();
        ui->lineEdit_invCredit->hide();

        ui->label_openInvKg->hide();
        ui->lineEdit_invKg->hide();

        ui->label_phone->hide();
        ui->lineEdit_phone->hide();

        ui->label_preparedBy->hide();
        ui->lineEdit_preparedBy->hide();

        ui->label_dif->hide();
        ui->lineEdit_navyDif->hide();
        ui->toolButton_navyDif->hide();
        ui->label_haulerNumber->show();
        ui->lineEdit_haulerNumber->show();
        ui->label_claim_collector->hide();
        ui->comboBox_claim_collector->hide();
        ui->formLayout->setSpacing(-1);
        setFixedHeight(350);
    }
    else if(ui->comboBox_action->currentIndex()==3||ui->comboBox_action->currentIndex()==4) {
        ui->label_username->hide();
        ui->lineEdit_username->hide();
        ui->label_password->hide();
        ui->lineEdit_password->hide();

        ui->label_claimPeriod->hide();
        ui->comboBox_month->hide();
        ui->comboBox_year->hide();
        ui->label_openInvCredit->hide();
        ui->lineEdit_invCredit->hide();

        ui->label_openInvKg->hide();
        ui->lineEdit_invKg->hide();

        ui->label_phone->hide();
        ui->lineEdit_phone->hide();

        ui->label_preparedBy->hide();
        ui->lineEdit_preparedBy->hide();

        ui->label_dif->show();
        ui->lineEdit_navyDif->show();
        ui->toolButton_navyDif->show();

        ui->label_haulerNumber->hide();
        ui->lineEdit_haulerNumber->hide();
        ui->label_claim_collector->setVisible(ui->comboBox_action->currentIndex()==4);
        ui->comboBox_claim_collector->setVisible(ui->comboBox_action->currentIndex()==4);
        setFixedHeight(350);
        ui->formLayout->setSpacing(1);
        adjustSize();
        page->setComplete(QFile(ui->lineEdit_navyDif->text()).exists());
        return;
    }
    else
    {
        ui->label_username->show();
        ui->lineEdit_username->show();
        ui->label_password->show();
        ui->lineEdit_password->show();

        ui->label_claimPeriod->show();
        ui->comboBox_month->show();
        ui->comboBox_year->show();

        ui->label_openInvCredit->show();
        ui->lineEdit_invCredit->show();

        ui->label_openInvKg->show();
        ui->lineEdit_invKg->show();

        ui->label_phone->show();
        ui->lineEdit_phone->show();

        ui->label_preparedBy->show();
        ui->lineEdit_preparedBy->show();

        ui->label_dif->hide();
        ui->lineEdit_navyDif->hide();
        ui->toolButton_navyDif->hide();
        ui->label_haulerNumber->show();
        ui->lineEdit_haulerNumber->show();
        ui->label_claim_collector->hide();
        ui->comboBox_claim_collector->hide();
        ui->formLayout->setSpacing(-1);
        setFixedHeight(430);
        if(ui->lineEdit_invCredit->text()==""||ui->lineEdit_invKg->text()==""||ui->lineEdit_phone->text()==""||ui->lineEdit_preparedBy->text()=="")
        {
            page->setComplete(0);
            return;
        }
    }

    if(ui->lineEdit_haulerNumber->text()=="")
    {
        page->setComplete(0);
        return;
    }
    if(ui->lineEdit_password->text()=="")
    {
        page->setComplete(0);
        return;
    }
    if(ui->lineEdit_username->text()=="")
    {
        page->setComplete(0);
        return;
    }

    page->setComplete(1);
}

void EWizard::showEula()
{
    EEULA eula(this);
    eula.setAutoFillBackground(1);
    eula.setPalette(this->palette());
}

void EWizard::autoFillPassword()
{
    for(int i=0;i<s_keys.size();i++)
    {
        QString substr=s_keys[i];
        substr.truncate(substr.indexOf(':'));
        QString psswd=s_keys[i];
        psswd.remove(0,psswd.indexOf(':')+1);
        if(ui->lineEdit_username->text()==substr)
        {
            ui->lineEdit_password->setText(psswd);
        }
    }
}

void EWizard::browseForDif()
{
    ui->lineEdit_navyDif->setText(QFileDialog::getOpenFileName(this,"Open NAVY DIF File",QString(QDir::homePath()),"*.dif"));
}

void EWizard::pageChangeLogic(int p)
{
    if(p==2)
    {
        s_browser.hide();
    }
    button(EWizard::BackButton)->setEnabled(1);
    if(!p&&s_action)
    {
        delete s_action;
        delete s_dif;
        s_action=0;
        s_dif=0;
        validatePage0();
        return;
    }
    else if(!p)
    {
        validatePage0();
        return;
    }
    if(p==1)
    {
        EWizardPage* page=static_cast<EWizardPage*>(this->page(1));
        page->setComplete(0);
        button(EWizard::BackButton)->setEnabled(0);

        if(ui->comboBox_action->currentIndex()==1||ui->comboBox_action->currentIndex()==3||ui->comboBox_action->currentIndex()==4)
        {
            s_dif=new EDifReader(ui->lineEdit_navyDif->text());
            connect(s_dif,SIGNAL(error(QString)),this,SLOT(error(QString)));
            s_dif->init();
        }
        if(ui->comboBox_action->currentIndex()==0)
        {
            if(QMessageBox::question(this,"Are you sure?","Are you sure you want to create a new claim?\n\nThis cannot be undone!",
                                  QMessageBox::Yes, QMessageBox::No)==QMessageBox::No)
            {
                restart();
                return;
            }
        }
        if(ui->comboBox_action->currentIndex()==3)
        {
            ui->log->setText("<B>Updating login information...</B>\n");
            s_action=new ECollLoginInfoAction(s_browser,*s_dif);
            connect(s_action,SIGNAL(done()),this,SLOT(next()));
            connect(s_action,SIGNAL(done()),this,SLOT(collLoginActionDone()));
            connect(s_action,SIGNAL(error(QString)),this,SLOT(error(QString)));
            connect(s_action,SIGNAL(info(QString)),this,SLOT(info(QString)));
            s_action->init();
            return;
        }
        else if(ui->comboBox_action->currentIndex()==4) {
            ui->log->setText("<B>Submitting information...</B>\n");
            // SUB PERIOD
            ECollAction::SubPeriod sp;
            switch(ui->comboBox_claim_collector->currentIndex()) {
            case 0:
                sp=ECollAction::Adj_Jul12_to_Sep;
                break;
            case 1:
                sp=ECollAction::Oct12_to_Dec12;
                break;
            case 2:
                sp=ECollAction::Adj_Oct12_to_Dec12;
                break;
            case 3:
                sp=ECollAction::Jan13_to_Mar13;
                break;
            case 4:
                QMessageBox::critical(this,"Invalid Period","An invalid period was detected. Try again or contact joshua@nettek.ca for support.");
                exit(0);
                break;
            }

            s_action=new EColDifAction(s_browser,*s_dif,sp);
            connect(s_action,SIGNAL(done()),this,SLOT(next()));
            connect(s_action,SIGNAL(info(QString)),this,SLOT(info(QString)));
            connect(s_action,SIGNAL(error(QString)),this,SLOT(error(QString)));
            s_action->init();
        }
        else
        {
            ui->log->setText("<B>Logging in...</B>");
            //logon
            s_action=new ELoginAction(s_browser,ui->lineEdit_username->text(),ui->lineEdit_password->text());
            connect(s_action,SIGNAL(done()),this,SLOT(loginDone()));
            connect(s_action,SIGNAL(error(QString)),this,SLOT(error(QString)));
        }
    }
}

void EWizard::loginDone()
{
    ui->log->setText(ui->log->text()+"Done!<br>");
    delete s_action;
    s_action=new EHaulerValidateAction(s_browser,ui->lineEdit_haulerNumber->text());
    connect(s_action,SIGNAL(done()),this,SLOT(haulerValidationDone()));
    connect(s_action,SIGNAL(error(QString)),this,SLOT(error(QString)));
    ui->log->setText(ui->log->text()+"<B>Validating hauler number...</B>");
}

void EWizard::haulerValidationDone()
{
    ui->log->setText(ui->log->text()+"Done!<br>");
    delete s_action;
    s_action=0;
    if(ui->comboBox_action->currentIndex()==0)
    {
        s_action=new ENewClaimAction(s_browser,ui->comboBox_month->currentIndex()+1,ui->comboBox_year->currentText().toInt(),ui->lineEdit_preparedBy->text(),
                                     ui->lineEdit_phone->text(),ui->lineEdit_invKg->text(),ui->lineEdit_invCredit->text());
        connect(s_action,SIGNAL(done()),this,SLOT(next()));
        connect(s_action,SIGNAL(error(QString)),this,SLOT(error(QString)));
        ui->log->setText(ui->log->text()+"<B>Creating Claim...</B>");
    }
    else if(ui->comboBox_action->currentIndex()==1||ui->comboBox_action->currentIndex()==2)
    {
        s_action=new ESelectClaimAction(s_browser);
        connect(s_action,SIGNAL(done()),this,SLOT(userClaimSelectionDone()));
        connect(s_action,SIGNAL(error(QString)),this,SLOT(error(QString)));
        ui->log->setText(ui->log->text()+"<B>Asking user to select claim...</B>");
    }
    else
    {
        error("Invalid action!");
    }
}

void EWizard::userClaimSelectionDone()
{
    if(ui->comboBox_action->currentIndex()==2)
    {
        delete s_action;
        s_action = new EDeleteClaimsAction(s_browser);
        QMessageBox::information(this,"Delete Claims","Please go to the page with the claims you want to delete. Deletion will start automatically.",QMessageBox::Ok);
        s_browser.show();
        connect(s_action,SIGNAL(done()),this,SLOT(next()));
        connect(s_action,SIGNAL(error(QString)),this,SLOT(error(QString)));
        connect(s_action,SIGNAL(info(QString)),this,SLOT(info(QString)));
        return;
    }
    ui->log->setText(ui->log->text()+"Done!<br>");
    delete s_action;
    s_action=0;
    ui->log->setText(ui->log->text()+"<B>Entering table data...</B>");
    s_action=new EDIFAction(s_browser,*s_dif);
    connect(s_action,SIGNAL(done()),this,SLOT(difActionDone()));
    connect(s_action,SIGNAL(done()),this,SLOT(next()));
    connect(s_action,SIGNAL(error(QString)),this,SLOT(error(QString)));
    connect(s_action,SIGNAL(info(QString)),this,SLOT(info(QString)));
    s_action->init();
}

void EWizard::difActionDone()
{

}

void EWizard::error(QString error)
{
    delete s_action;
    s_action=0;
    QMessageBox::critical(this,"Error",error);
    restart();
}

void EWizard::collLoginActionDone()
{
    ui->label->setText("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\"><html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">p, li { white-space: pre-wrap; }</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\"><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Your login information has been updated!</span></p><p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">If you experienced any problems with the upload, please contact the developper at <span style=\" text-decoration: underline;\">joshua@nettek.ca</span>.</p><p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Thank you for using Data Import for the Ontario Tire Stewardship.</p></body></html>");
}

void EWizard::info(QString info)
{
    QString text=ui->log->text();
    text.truncate(text.lastIndexOf("..."));
    ui->log->setText(text+"..."+info);
}

void EWizard::showHelp()
{
    EWizard*me=this;

    EHelpDialog help(me);
    help.setAutoFillBackground(1);
    help.setPalette(this->palette());

    help.exec();
    return;
}
