/****************************************************
ecolllogininfoaction.cpp

   Part of Data Import for Ontario Tire Stewardship
        Copyright (C) Joshua Netterfield 2012

                 All rights reserved.
*****************************************************/

#include "ecolllogininfoaction.h"
#include <QSettings>

ECollLoginInfoAction::ECollLoginInfoAction(EBrowser &browser, EDifReader &reader) : EAction(browser), s_reader(reader)
{
}

void ECollLoginInfoAction::continueAction()
{
    QSettings sett("Nettek","Data Import for Ontario Tire Stewardship");
    QVariantMap map=sett.value("CollectorMap").toMap();

    if(s_reader.columnCount()<3) {
        emit error("A collector login information DIF file must have at least 3 columns. Contact joshua@nettek.ca for support.");
    } else if(s_reader.rowCount()<3) {
        emit error("A collector login information DIF file must have at least 3 rows. Contact joshua@nettek.ca for support.");
    }
    for(int i=2;i<s_reader.rowCount();i++) {
        QStringList u_p;
        u_p.push_back(s_reader.getData(1,i));
        u_p.push_back(s_reader.getData(2,i));
        map.insert(s_reader.getData(0,i),u_p);
    }
    sett.setValue("CollectorMap",map);
    emit done();
}
