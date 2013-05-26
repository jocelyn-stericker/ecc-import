/****************************************************
eselectclaimaction.cpp

   Part of Data Import for Ontario Tire Stewardship
        Copyright (C) Joshua Netterfield 2012

                 All rights reserved.
*****************************************************/

#include "eselectclaimaction.h"
#include <QVariant>
#include <QDebug>
#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>

ESelectClaimAction::ESelectClaimAction(EBrowser &browser) : EAction(browser)
{
    browser.askQuestion("Please select a claim with \"New\" status.");
    QStringList html=browser.html().split('\n');
    QStringList claims;
    QStringList actions;
    for(int i=0;i<html.size();i++)
    {
        if(html[i].contains(";\">view</a>")&&html[i].contains("New"))
        {
            QString tmp=html[i];
            tmp.remove(0,tmp.indexOf("<a href=\"javascript:")+20);
            tmp.truncate(tmp.indexOf(";\">view</a>"));
            actions.push_back(tmp);
            tmp=html[i];
            tmp.remove(0,tmp.indexOf("</a></td><td>")+13);
            tmp.truncate(tmp.indexOf("</td><td "));
            claims.push_back(tmp);
            tmp=html[i];
            tmp.remove(0,tmp.indexOf("hauler-num\">")+12);
            tmp.truncate(tmp.indexOf("</td><td>"));
            claims.back()+=", Submission "+tmp;
        }
    }

    if(claims.removeDuplicates())
    {
        emit error("Duplicate claim names/submission numbers found. Please try again or email joshua@nettek.ca for support.");
    }

    if(!claims.size())
    {
        emit error("There are currently no claims in the account you selected.");
        return;
    }
    else
    {
        bool ok;
        QString response=QInputDialog::getItem(0,"Select Claim to Edit","Claim:",claims,0,0,&ok);
        if(!ok)
        {
            emit error("You must select a claim to edit.");
            return;
        }
        browser.doJS(actions[claims.indexOf(response)]);
    }

    connect(&browser,SIGNAL(ready()),this,SLOT(continue1()));
}

void ESelectClaimAction::continue1()
{
    RUN_SECOND_TIME

    s_browser.askQuestion("");
    disconnect(&s_browser,SIGNAL(ready()),this,SLOT(continue1()));
    if(s_browser.htmlContains("Under Review"))
    {
        emit error("The claim you selected is under review and cannot be modified.");
    }
    else
    {
        emit done();
    }
}
