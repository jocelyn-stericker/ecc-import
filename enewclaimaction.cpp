/****************************************************
enewclaimaction.cpp

   Part of Data Import for Ontario Tire Stewardship
        Copyright (C) Joshua Netterfield 2012

                 All rights reserved.
*****************************************************/

#include "enewclaimaction.h"
#include <QTimer>

ENewClaimAction::ENewClaimAction(EBrowser &browser,int claimMonth,int claimYear,QString preparedBy,QString phoneNumber,QString invWeight,QString invCredit) : EAction(browser),s_notDone(1)
{
    browser.setInput("edit-period-month",QString::number(claimMonth));
    browser.setInput("edit-period-year",QString::number(claimYear));
    browser.setInput("edit-prepared-by",preparedBy);
    browser.setInput("edit-phone",phoneNumber);
    browser.setInput("edit-opening-inv-weight",invWeight);
    browser.setInput("edit-opening-inv-credit",invCredit);
    browser.clickInput("edit-submit");
    connect(&browser,SIGNAL(ready()),this,SLOT(continue1()));
    QTimer::singleShot(25000,this,SLOT(fail()));
}

void ENewClaimAction::continue1()
{
    s_notDone=0;
    emit done();
}

void ENewClaimAction::fail()
{
    if(s_notDone)
    {
        emit error("Could not create new claim.");
    }
}
