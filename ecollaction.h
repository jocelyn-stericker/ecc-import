/****************************************************
ecollaction.cpp

   Part of Data Import for Ontario Tire Stewardship
        Copyright (C) Joshua Netterfield 2012

                 All rights reserved.
*****************************************************/

#ifndef ECOLLACTION_H
#define ECOLLACTION_H

#include "eaction.h"
#include "edifreader.h"

class ECollAction : public EAction
{
    Q_OBJECT
public:
    enum SubPeriod {
        Adj_Jan12_to_Mar12=33,
        Apr_01_2012_to_Jun_30_2012=34,
        Adj_Apr12_to_Jun12=35,
        Jul_01_2012_to_Sep_30_2012=39,
        Adj_Jul12_to_Sep=42,
        Oct12_to_Dec12=43,
        Adj_Oct12_to_Dec12=45,
        Jan13_to_Mar13=48
    };
private:
    QString s_username, s_password, s_collector;
    SubPeriod s_subPeriod;
    int s_day, s_month, s_year;
    QString s_tcrOrDot;
    QString s_formNumber;
    QString s_haulerNumber;
    bool s_genTires;
    int s_plt;
    int s_mt;
    int s_ag, s_ind, s_sotr, s_motr, s_lotr, s_gotr;
    bool s_eligForPay;
    int s_row;
    EAction* s_subAction;
    static QString s_lastUsername;
public:
    ECollAction(EBrowser&browser,
                QString username,
                QString password,
                QString collector,
                SubPeriod subPeriod,
                int day, int month, int year,
                QString tcrOrDot,
                QString formNumber,
                QString haulerNumber,
                bool genTires,
                int plt,
                int mt,
                int ag, int ind, int sotr, int motr, int lotr, int gotr,
                bool eligForPay,
                int row);

public slots:
    void init();
    void stage2();
    void stage3();
    void stage4();
    void stage5();
    void stage6();
};

class EColDifAction : public EAction {
    Q_OBJECT
    int i;
    EDifReader& s_dif;
    ECollAction::SubPeriod sp;
    EAction* s_action;
public:
    EColDifAction(EBrowser& browser, EDifReader& reader, ECollAction::SubPeriod p);
public slots:
    void init() { nextRow(true); }
    void nextRow(bool strict = false);
};

#endif // ECOLLACTION_H
