/****************************************************
edeleteclaimsaction.h

   Part of Data Import for Ontario Tire Stewardship
        Copyright (C) Joshua Netterfield 2012

                 All rights reserved.
*****************************************************/

#include "edeleteclaimsaction.h"
#include <QDebug>

//javascript:remove_inventory_record('30357');
EDeleteClaimsAction::EDeleteClaimsAction(EBrowser& browser) : EAction(browser), s_loading(1)
{
    connect(&s_browser,SIGNAL(ready()),this,SLOT(tryDelete()));
    tryDelete();
    emit info("Waiting for info");
}

void EDeleteClaimsAction::tryDelete()
{
    RUN_SECOND_TIME

    QStringList html=s_browser.html().split('\n');
    QStringList actions;
    for(int i=0;i<html.size();i++)
    {
        if(html[i].contains("javascript:remove_inventory_record('"))
        {
            QString tmp=html[i];
            tmp.remove(0,tmp.indexOf("javascript:remove_inventory_record('"));
            tmp.truncate(tmp.indexOf("');")+3);
            actions.push_back(tmp);
            qDebug()<<"GOT JS"<<tmp;
            s_browser.doJS(tmp);
            s_loading=0;
            emit info("Removed a claim...");
            return;
        }
    }
    if(!s_loading)
    {
        emit done();
    }
}
