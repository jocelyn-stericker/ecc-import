/****************************************************
eloginaction.h

   Part of Data Import for Ontario Tire Stewardship
        Copyright (C) Joshua Netterfield 2012

                 All rights reserved.
*****************************************************/

#ifndef ELOGINACTION_H
#define ELOGINACTION_H

#include "eaction.h"

class ELoginAction : public EAction
{
    Q_OBJECT
    QString s_username;
    QString s_password;
public:
    ELoginAction(EBrowser&browser,QString username,QString password);
public slots:
    void continue1();
    void continue2();
    void continue3();
};

#endif // ELOGINACTION_H
