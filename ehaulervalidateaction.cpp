/****************************************************
edifreader.cpp

   Part of Data Import for Ontario Tire Stewardship
        Copyright (C) Joshua Netterfield 2012

                 All rights reserved.
*****************************************************/

#include "ehaulervalidateaction.h"
#include <QSettings>

EHaulerValidateAction::EHaulerValidateAction(EBrowser &browser, const QString &regNumber) : EAction(browser), s_regNumber(regNumber)
{
    browser.loadPage("http://www.ontariots.ca//?q=haulerwebform/validate");
    connect(&browser,SIGNAL(ready()),this,SLOT(continue1()));
}

void EHaulerValidateAction::continue1()
{
    disconnect(&s_browser,SIGNAL(ready()),this,SLOT(continue1()));
    if(s_browser.htmlContains("Your online OTS Hauler Account is not activated for this Web User ID"))
    {
        emit error("Your online OTS Hauler Account is not activated for this Web User ID");
        return;
    }
    s_browser.setInput("edit-hauler-reg-no",s_regNumber);
    s_browser.clickInput("edit-submit");
    connect(&s_browser,SIGNAL(ready()),this,SLOT(continue2()));
}

void EHaulerValidateAction::continue2()
{
    RUN_SECOND_TIME

    disconnect(&s_browser,SIGNAL(ready()),this,SLOT(continue2()));
    if(s_browser.htmlContains("Your online OTS Hauler Account is not activated for this Web User ID"))
    {
        emit error("Your online OTS Hauler Account is not activated for this Web User ID");
        return;
    }
    else if(s_browser.htmlContains("<legend>Claim Form</legend>"))
    {
        QSettings settings("Nettek","Data Import for Ontario Tire Stewardship");
        QStringList keys=settings.value("HaulerNums",QStringList()).toStringList();
        for(int i=0;i<keys.size();i++)
        {
            if(keys.at(i)==s_regNumber)
            {
                keys.removeAt(i);
                --i;
            }
        }
        keys.push_back(s_regNumber);
        settings.setValue("HaulerNums",keys);
        emit done();
    }
    else
    {
        emit error("An unknown error occured while validating the hauler number.");
    }
}
