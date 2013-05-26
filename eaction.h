/****************************************************
eaction.h

   Part of Data Import for Ontario Tire Stewardship
        Copyright (C) Joshua Netterfield 2012

                 All rights reserved.
*****************************************************/

#ifndef EACTION_H
#define EACTION_H

#include <QObject>
#include "ebrowser.h"

#define RUN_SECOND_TIME \
static bool m_ok = false; \
if (!m_ok) { \
    m_ok = true; \
    return; \
} \
m_ok = false;

class EAction : public QObject
{
    Q_OBJECT
protected:
    EBrowser& s_browser;
public:
    EAction(EBrowser& browser);
    virtual void init() {}
signals:
    void done();
    void error(QString);
    void info(QString);
};

#endif // EACTION_H
