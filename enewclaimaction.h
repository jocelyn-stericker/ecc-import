/****************************************************
enewclaimaction.cpp

   Part of Data Import for Ontario Tire Stewardship
        Copyright (C) Joshua Netterfield 2012

                 All rights reserved.
*****************************************************/

#ifndef ENEWCLAIMACTION_H
#define ENEWCLAIMACTION_H

#include "eaction.h"

class ENewClaimAction : public EAction
{
    Q_OBJECT
    bool s_notDone;
public:
    ENewClaimAction(EBrowser&browser,int claimMonth,int claimYear,QString preparedBy,QString phoneNumber,QString invWeight,QString invCredit);
public slots:
    void continue1();
    void fail();
};

#endif // ENEWCLAIMACTION_H
