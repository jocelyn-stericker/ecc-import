/****************************************************
edifaction.cpp

   Part of Data Import for Ontario Tire Stewardship
        Copyright (C) Joshua Netterfield 2012

                 All rights reserved.
*****************************************************/

#ifndef EDIFACTION_H
#define EDIFACTION_H

#include "eaction.h"
#include "edifreader.h"
#include <QStringList>

class EDIFAction : public EAction
{
    Q_OBJECT
    EDifReader& s_reader;
    int s_curRow;
    int s_curCol;
    QStringList s_colActions;
    bool s_rowBlank;
public:
    EDIFAction(EBrowser& browser, EDifReader& reader);
public slots:
    void init() { continueAction(true); }
    void continueAction(bool strict=false);
};

#endif // EDIFACTION_H
