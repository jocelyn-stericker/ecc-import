/****************************************************
eselectclaimaction.h

   Part of Data Import for Ontario Tire Stewardship
        Copyright (C) Joshua Netterfield 2012

                 All rights reserved.
*****************************************************/

#ifndef ESELECTCLAIMACTION_H
#define ESELECTCLAIMACTION_H

#include "eaction.h"

class ESelectClaimAction : public EAction
{
    Q_OBJECT
public:
    ESelectClaimAction(EBrowser& browser);
public slots:
    void continue1();
};

#endif // ESELECTCLAIMACTION_H
