/****************************************************
edifreader.cpp

   Part of Data Import for Ontario Tire Stewardship
        Copyright (C) Joshua Netterfield 2012

                 All rights reserved.
*****************************************************/

#ifndef EEULA_H
#define EEULA_H

#include <QDialog>

namespace Ui {
    class EEULA;
}

class EEULA : public QDialog
{
    Q_OBJECT

public:
    explicit EEULA(QWidget *parent = 0);
    ~EEULA();

private:
    Ui::EEULA *ui;
};

#endif // EEULA_H
