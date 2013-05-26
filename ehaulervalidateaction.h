/****************************************************
edifreader.cpp

   Part of Data Import for Ontario Tire Stewardship
        Copyright (C) Joshua Netterfield 2012

                 All rights reserved.
*****************************************************/

#ifndef EHAULERVALIDATEACTION_H
#define EHAULERVALIDATEACTION_H

#include "eaction.h"

class EHaulerValidateAction : public EAction
{
    Q_OBJECT
    QString s_regNumber;
public:
    EHaulerValidateAction(EBrowser& browser, const QString& regNumber);
public slots:
    void continue1();
    void continue2();
};

#endif // EHAULERVALIDATEACTION_H
