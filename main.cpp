/****************************************************
main.cpp

   Part of Data Import for Ontario Tire Stewardship
        Copyright (C) Joshua Netterfield 2012

                 All rights reserved.
*****************************************************/

#include <QtGui/QApplication>
#include <QSharedMemory>
#include <QMessageBox>
#include "ewizard.h"
#include "edifreader.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qApp->addLibraryPath("C:/Program Files/Data Import for OTS/plugins");
//    QSharedMemory mem("dataImportForOTS");
//    if(!mem.create(12))
//    {
//        QMessageBox::critical(0,"Already running","Only one instance of Data Import for Ontario Tire Stewardship can run at a time");
//        return 1;
//    }

//    EDifReader reader("/home/joshua/Downloads/template.dif");
    EWizard w;
    w.show();

    a.exec();
//    mem.detach();
    return 0;
}
