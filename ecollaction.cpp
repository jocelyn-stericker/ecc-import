/****************************************************
ecollaction.cpp

   Part of Data Import for Ontario Tire Stewardship
        Copyright (C) Joshua Netterfield 2012

                 All rights reserved.
*****************************************************/

#include "ecollaction.h"
#include "eloginaction.h"

#include <QSettings>
#include <QMessageBox>
#include <QEventLoop>
#include <QTimer>

QString ECollAction::s_lastUsername;

ECollAction::ECollAction(EBrowser&browser, QString username, QString password, QString collector, SubPeriod subPeriod, int day, int month, int year, QString tcrOrDot, QString formNumber, QString haulerNumber, bool genTires, int plt, int mt,
                         int ag, int ind, int sotr, int motr, int lotr, int gotr, bool eligForPay, int row) :
    EAction(browser), s_username(username), s_password(password), s_collector(collector), s_subPeriod(subPeriod), s_day(day), s_month(month), s_year(year), s_tcrOrDot(tcrOrDot), s_formNumber(formNumber), s_haulerNumber(haulerNumber), s_genTires(genTires), s_plt(plt),
    s_mt(mt), s_ag(ag), s_ind(ind), s_sotr(sotr), s_motr(motr), s_lotr(lotr), s_gotr(gotr), s_eligForPay(eligForPay), s_row(row), s_subAction(0)
{
}

void ECollAction::init()
{
    if(s_lastUsername==s_username) {
        stage2();
    } else {
        s_lastUsername=s_username;
        emit info("Row "+QString::number(s_row+1)+": Logging in");
        s_subAction=new ELoginAction(s_browser,s_username,s_password);
        connect(s_subAction,SIGNAL(error(QString)),this,SIGNAL(error(QString)));
        connect(s_subAction,SIGNAL(info(QString)),this,SIGNAL(info(QString)));
        connect(s_subAction,SIGNAL(done()),this,SLOT(stage2()));
    }
}

void ECollAction::stage2()
{
    emit info("Row "+QString::number(s_row+1)+": Searching for registration number");
    if(s_subAction) s_subAction->deleteLater();
    s_subAction=0;
    s_browser.loadPage("http://www.ontariots.ca//?q=collectorswebform/form/search");
    connect(&s_browser,SIGNAL(ready()),this,SLOT(stage3()));
}

void ECollAction::stage3()
{
    RUN_SECOND_TIME

    disconnect(&s_browser,SIGNAL(ready()),this,SLOT(stage3()));
    s_browser.setInput("edit-registration-number",s_collector);
    s_browser.clickInput("edit-searchbtn");
    connect(&s_browser,SIGNAL(ready()),this,SLOT(stage4()));
}

void ECollAction::stage4()
{
    RUN_SECOND_TIME

    emit info("Row "+QString::number(s_row+1)+": Selecting claim period");
    disconnect(&s_browser,SIGNAL(ready()),this,SLOT(stage4()));
    if(s_browser.htmlContains("We are not able to find")&&s_browser.htmlContains("registration number as registered collector")) {
        emit error("Invalid registration number on row "+QString::number(s_row+1)+". Quitting. Data may have been submitted. Please review file on the OTS website");
        return;
    }
    if(!s_browser.htmlContains("Step 6 - Click ")) {
        emit error("An unknown error occured while trying to bring up collector form. Data may have been supported. Try again or contact support at joshua@nettek.ca!");
        return;
    }
    s_browser.setInput("edit-claim-period",QString::number((int)s_subPeriod));
    s_browser.clickInput("edit-next");
    connect(&s_browser,SIGNAL(ready()),this,SLOT(stage5()));
}

void ECollAction::stage5()
{
    RUN_SECOND_TIME

    disconnect(&s_browser,SIGNAL(ready()),this,SLOT(stage5()));
    emit info("Row "+QString::number(s_row+1)+": Entering information");
    disconnect(&s_browser,SIGNAL(ready()),this,SLOT(stage5()));
    if(s_browser.htmlContains("Please select a period")) {
        emit error("The period you selected is invalid, or the website has changed. Try again or contact support at joshua@nettek.ca!");
        return;
    }
    if(s_browser.htmlContains("as its already been submitted for this claim period.")) {
        emit error("A claim has already been submitted for the period you selected!");
        return;
    }
    if(!s_browser.htmlContains("Please report outbound inventory documented on TCR and DOT forms.")) {
        emit error("An unknown error has occured! Try again or contact support at joshua@nettek.ca!");
        return;
    }
    s_browser.setInput("edit-date-collected-month",QString::number(s_month));
    qDebug()<<"!!YEAR"<<"20"+QString::number(s_year)+"#";
    if(QString::number(s_year).startsWith("20")) s_browser.setInput("edit-date-collected-year",QString::number(s_year));
    else s_browser.setInput("edit-date-collected-year","20"+QString::number(s_year));
    s_browser.setInput("edit-date-collected-day",QString::number(s_day));

    s_browser.setInput("edit-form-type",s_tcrOrDot.toLower());
    s_browser.setInput("edit-form-number",s_formNumber);
    s_browser.setInput("edit-hauler-number",s_haulerNumber);
    s_browser.setInput("edit-generated-tires",s_genTires?"y":"n");
    s_browser.setInput("edit-elig-for-pay",s_genTires?"n":"y");

    s_browser.setInput("edit-plt",QString::number(s_plt));
    s_browser.setInput("edit-mt",QString::number(s_mt));
    s_browser.setInput("edit-ag-ls",QString::number(s_ag));
    s_browser.setInput("edit-ind",QString::number(s_ind));
    s_browser.setInput("edit-sotr",QString::number(s_sotr));
    s_browser.setInput("edit-motr",QString::number(s_motr));
    s_browser.setInput("edit-lotr",QString::number(s_lotr));
    s_browser.setInput("edit-gotr",QString::number(s_gotr));

    s_browser.setInput("edit-elig-for-pay",s_eligForPay?"y":"n");

    connect(&s_browser,SIGNAL(ready()),this,SLOT(stage6()));

    QEventLoop loop;
    QTimer::singleShot(200,&loop,SLOT(quit()));
    loop.exec();

    s_browser.clickInput("edit-save");
}

void ECollAction::stage6()
{
    emit info("Row "+QString::number(s_row+1)+": Finalizing");
//    RUN_SECOND_TIME

    disconnect(&s_browser,SIGNAL(ready()),this,SLOT(stage6()));
    if(s_browser.htmlContains("Warning: The entered date does not match our records")) {
        emit error("The entered date for row "+QString::number(s_row+1)+" does not match OTS' records. Data may have been submitted. Please verify the file on the OTS website. ");
    } else if (s_browser.htmlContains("Date collected is not within the claim period.")) {
        emit error("The entered date for row "+QString::number(s_row+1)+" is not within the claim period. Data may have been submitted. Please verify the file on the OTS website. ");
    } else if (s_browser.htmlContains("already exists in your current submission. Please verify.")) {
        QMessageBox::information(0,"Already Submitted","This program tried to modify a claim which was already submitted. If you wish to change it, you must do so manually. Please contact joshua@nettek.ca for details.");
        emit done();
    } else if(s_browser.htmlContains("<div class=\"messages error\">")) {
        emit error("An unknown error occured");
    } else {
        emit done();
    }
}


EColDifAction::EColDifAction(EBrowser& browser, EDifReader& reader, ECollAction::SubPeriod p) : EAction(browser), i(3), s_dif(reader), sp(p), s_action(0) {}

void EColDifAction::nextRow(bool strict) {
    if (!strict) {
//        RUN_SECOND_TIME
    }

    if(s_action) disconnect(s_action,SIGNAL(done()),this,SLOT(nextRow()));

    qDebug()<<"NEXT ROW";
    ++i;
    if(i==s_dif.rowCount()) {
        emit done();
        return;
    }

    QSettings sett("Nettek","Data Import for Ontario Tire Stewardship");
    QVariantMap vmap=sett.value("CollectorMap").toMap();

    if(s_dif.columnCount()<17) {
        emit error("A collector claim DIF file must have at 17 columns. Contact joshua@nettek.ca for support.");
    } else if(s_dif.rowCount()<5) {
        emit error("A collector claim DIF file must have at least 5 rows (1 entry). Contact joshua@nettek.ca for support.");
    }

    if(s_dif.getData(0,i).isEmpty()) {
        nextRow(true);
        return;
    }
    qDebug()<<i;
    // COLLECTION DATE
    QString procStr=s_dif.getData(1,i);
    QString map[12]={"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};
    QStringList args=procStr.split('-');
    if(args.size()!=3)
    {
        emit error("Malformed date string on row "+QString::number(i+1)+". Got " + procStr + " which does not have 3 parts. Syntax is DD-MMM-YY. Data may have been submitted. "
                   "Please review file.");
        return;
    }
    int month=-1;
    for(int j=0;j<12;j++)
    {
        if(args[1].toUpper()==map[j])
        {
            month=j+1;
            break;
        }
    }
    if(month==-1)
    {
        emit error("Malformed date string on row "+QString::number(i+1)+". Syntax is DD-MMM-YY. MMM must be one of JAN,FEB,MAR,APR,MAY,"
                   "JUN,JUL,AUG,SEP,OCT,NOV OR DEC (case insensitive). Data may have been submitted. Please review file.");
        return;
    }
    while(args.at(0).startsWith("0"))
    {
        args[0].remove(0,1);
    }

    if(vmap.value(s_dif.getData(0,i)).isNull()) {
        emit error("Login information for the user "+s_dif.getData(0,i)+"is missing. Please update your collector login information and try again. "
                   "If this error persists, contact joshua@nettek.ca. Data may have been submitted.");
        return;
    }

    s_action=new ECollAction(s_browser,
                             vmap.value(s_dif.getData(0,i)).toStringList()[0],
                             vmap.value(s_dif.getData(0,i)).toStringList()[1],
                             s_dif.getData(0,i),
                             sp,
                             args[0].toInt(),
                             month,
                             args[2].toInt(), /*looks ok*/
                             s_dif.getData(3,i),
                             s_dif.getData(4,i),
                             s_dif.getData(5,i),
                             ((s_dif.getData(7,i)=="Y")?1:0),
                             s_dif.getData(8,i).toInt(),
                             s_dif.getData(9,i).toInt(),
                             s_dif.getData(10,i).toInt(),
                             s_dif.getData(11,i).toInt(),
                             s_dif.getData(12,i).toInt(),
                             s_dif.getData(13,i).toInt(),
                             s_dif.getData(14,i).toInt(),
                             s_dif.getData(15,i).toInt(),
                             ((s_dif.getData(16,i)=="Y")?1:0),i);
    connect(s_action,SIGNAL(error(QString)),this,SIGNAL(error(QString)));
    connect(s_action,SIGNAL(info(QString)),this,SIGNAL(info(QString)));
    connect(s_action,SIGNAL(done()),this,SLOT(nextRow()));

    s_action->init();

}
