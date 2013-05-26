/****************************************************
eloginaction.cpp

   Part of Data Import for Ontario Tire Stewardship
        Copyright (C) Joshua Netterfield 2012

                 All rights reserved.
*****************************************************/

#include "eloginaction.h"
#include <QSettings>
#include <QDebug>

ELoginAction::ELoginAction(EBrowser &browser, QString username, QString password) : EAction(browser), s_username(username), s_password(password)
{
    browser.loadPage("http://www.ontariots.ca//?q=logout");  // ensure that we're logged out.
    connect(&browser,SIGNAL(ready()),this,SLOT(continue1()));
}

void ELoginAction::continue1()
{
    disconnect(&s_browser,SIGNAL(ready()),this,SLOT(continue1()));
    s_browser.loadPage("http://www.ontariots.ca//?q=user/login");
    connect(&s_browser,SIGNAL(ready()),this,SLOT(continue2()));
}

void ELoginAction::continue2()
{
    disconnect(&s_browser,SIGNAL(ready()),this,SLOT(continue2()));
    s_browser.setInput("edit-name",s_username);
    s_browser.setInput("edit-pass",s_password);
    s_browser.clickInput("edit-submit");
    connect(&s_browser,SIGNAL(ready()),this,SLOT(continue3()));
}

void ELoginAction::continue3()
{
    RUN_SECOND_TIME

    disconnect(&s_browser,SIGNAL(ready()),this,SLOT(continue3()));
    if(s_browser.htmlContains("Sorry, unrecognized username or password."))
    {
        qDebug()<<"Error logging in!";
        emit error("Sorry, unrecognized username or password.");
    }
    else if(s_browser.htmlContains("<li>Password field is required.</li>"))
    {
        qDebug()<<"Error logging in!";
        emit error("Login information is required.");
    }
    else if(s_browser.htmlContains("Welcome back!"))
    {
        qDebug()<<"Login complete!";
        QSettings settings("Nettek","Data Import for Ontario Tire Stewardship");
        QStringList keys=settings.value("Keys",QStringList()).toStringList();
        for(int i=0;i<keys.size();i++)
        {
            if(keys.at(i).startsWith(s_username+":"))
            {
                keys.removeAt(i);
                --i;
            }
        }
        keys.push_back(s_username+":"+s_password);
        settings.setValue("Keys",keys);
        emit done();
    }
    else
    {
        qDebug()<<"Error logging in!";
        emit error("An unknown error occured while logging in.");
    }
}

