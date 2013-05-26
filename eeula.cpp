/****************************************************
edifreader.cpp

   Part of Data Import for Ontario Tire Stewardship
        Copyright (C) Joshua Netterfield 2012

                 All rights reserved.
*****************************************************/

#include "eeula.h"
#include "ui_eeula.h"

EEULA::EEULA(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EEULA)
{
    ui->setupUi(this);
}

EEULA::~EEULA()
{
    delete ui;
}
