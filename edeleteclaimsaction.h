/****************************************************
edeleteclaimsaction.h

   Part of Data Import for Ontario Tire Stewardship
        Copyright (C) Joshua Netterfield 2012

                 All rights reserved.
*****************************************************/

#ifndef EDELETECLAIMSACTION_H
#define EDELETECLAIMSACTION_H

#include "eaction.h"

class EDeleteClaimsAction : public EAction
{
    Q_OBJECT
    bool s_loading;
public:
    EDeleteClaimsAction(EBrowser& s_browser);
public slots:
    void tryDelete();
};

#endif // EDELETECLAIMSACTION_H
