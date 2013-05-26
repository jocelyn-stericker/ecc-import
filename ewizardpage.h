/****************************************************
ewizardpage.h

   Part of Data Import for Ontario Tire Stewardship
        Copyright (C) Joshua Netterfield 2012

                 All rights reserved.
*****************************************************/

#ifndef EWIZARDPAGE_H
#define EWIZARDPAGE_H

#include <QWizardPage>
#include <QPushButton>
#include <QDebug>

class EWizardPage : public QWizardPage
{
    Q_OBJECT
    bool s_complete;
public:
    explicit EWizardPage(QWidget *parent = 0);

signals:

public slots:
    void setComplete(bool complete)
    {
        s_complete=complete;
        emit completeChanged();
    }

protected:
    bool isComplete() const
    {
        return s_complete;
    }

};

#endif // EWIZARDPAGE_H
