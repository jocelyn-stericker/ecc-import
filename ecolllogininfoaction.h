/****************************************************
ecolllogininfoaction.h

   Part of Data Import for Ontario Tire Stewardship
        Copyright (C) Joshua Netterfield 2012

                 All rights reserved.
*****************************************************/

#ifndef ECOLLLOGININFOACTION_H
#define ECOLLLOGININFOACTION_H

#include "eaction.h"
#include "edifreader.h"

class   ECollLoginInfoAction : public EAction
{
    Q_OBJECT
    EDifReader& s_reader;
public:
    ECollLoginInfoAction(EBrowser& browser, EDifReader& reader);
public slots:
    void init() { continueAction(); }
    void continueAction();
};

#endif // ECOLLLOGININFOACTION_H
