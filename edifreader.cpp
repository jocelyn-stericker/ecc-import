/****************************************************
edifreader.cpp

   Part of Data Import for Ontario Tire Stewardship
        Copyright (C) Joshua Netterfield 2012

                 All rights reserved.
*****************************************************/

#include "edifreader.h"
#include <QDebug>
#include <QMessageBox>
#include <QApplication>

QByteArray readLineNoEsc(const QByteArray& a)
{
    QByteArray ret(a);
    if(ret[ret.size()-1]=='\n')
    {
        ret.chop(1);
    }
    return ret;
}

EDifReader::EDifReader(const QString& path) : s_path(path), s_file(path), s_columns(-1), s_rows(-1)
{
}

void EDifReader::init()
{
    if(!s_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"EDifReader cannot open"<<s_path;
        emit error("Data Import for Ontario Tire Stewardship cannot open \""+s_path+
                   "\". Please ensure that the file exists, is not being accessed and that you have the appropriate permissions "
                   "to read the file.");
        s_good=0;
        return;
    }

    /* Parse the table. */

    /*
        TABLE
        0, < version # >
        " < title > "
    */
    if(readLineNoEsc(s_file.readLine())!="TABLE")
    {
        notifyInvalid();
        return;
    }
    readLineNoEsc(s_file.readLine());  //garbage
    readLineNoEsc(s_file.readLine());  //garbage

    /* Vector count = number of columns
        VECTORS
        0, < count >
        ""
    */
    if(readLineNoEsc(s_file.readLine())!="VECTORS")
    {
        notifyInvalid();
        return;
    }
    s_columns=readLineNoEsc(s_file.readLine()).remove(0,2).toInt();
    qDebug()<<s_path<<"has"<<s_columns<<"columns.";
    readLineNoEsc(s_file.readLine());  //garbage

    /* Tuple count = number of rows
        TUPLES
        0, < count >
        ""
    */
    if(readLineNoEsc(s_file.readLine())!="TUPLES")
    {
        notifyInvalid();
        return;
    }
    s_rows=readLineNoEsc(s_file.readLine()).remove(0,2).toInt();
    qDebug()<<s_path<<"has"<<s_rows<<"rows.";
    readLineNoEsc(s_file.readLine());  //garbage

    /*
        DATA
        0,0
        ""
    */
    if(readLineNoEsc(s_file.readLine())!="DATA")
    {
        notifyInvalid();
        return;
    }
    readLineNoEsc(s_file.readLine());  //garbage
    readLineNoEsc(s_file.readLine());  //garbage
    int curRow=-1,curCol=0;
    while(!s_file.atEnd())
    {
        QByteArray  ln=readLineNoEsc(s_file.readLine());
        QString lnb=ln;
        ln.remove(ln.indexOf(","),999999);    //i.e., inf
        lnb.remove(0,ln.indexOf(","));
        switch(ln.toInt())
        {
        case -1:
            ln=readLineNoEsc(s_file.readLine());
            if(ln=="BOT")
            {
                s_data.push_back(QStringList());
                ++curRow;
                curCol=0;
            }
            else if(ln=="EOD")
            {
                qDebug()<<"Finished reading file.";
                goto esc;   //don't hate me. double break.
            }
            else
            {
                notifyInvalid();
                return;
            }
            break;
        case 0:
            if(curRow==-1)
            {
                qDebug()<<"Invalid NULL row. Skipping...";
                continue;
            }
            ln=readLineNoEsc(s_file.readLine());
            s_data.back().push_back(lnb);
            s_data.back().back().remove(0,2);
//            qDebug()<<"Got num data"<<lnb;
            ++curCol;
            break;
        case 1:

            if(curRow==-1)
            {
                qDebug()<<"Invalid NULL row. Skipping...";
                continue;
            }
            ln=readLineNoEsc(s_file.readLine());
            ln.chop(1);
            ln.remove(0,1);
            ++curCol;
            s_data.back().push_back(ln);
//            qDebug()<<"Got str data"<<ln;
            break;
        }
    }

 esc:
    qDebug()<<"Done reading";
    s_columns=s_data[1].size();
    s_rows=s_data.size();
    qDebug()<<"NEW DIM"<<s_columns<<"X"<<s_rows;
//  for debug, output data
//    for(int i=0;i<s_columns;i++)
//    {
//        QString ln;
//        for(int j=0;j<s_rows;j++)
//        {
//            ln+=s_data[i][j]+"\t";
//        }
//        qDebug()<<ln;
//    }
}

void EDifReader::notifyInvalid()
{
    qDebug()<<s_path<<"does not seem to be a valid DIF file.";
    emit error("Data Import for Ontario Tire Stewardship cannot open \""+s_path+
               "\" because it is not a Data Interchange Format file. Please ensure to select a file in the Data "
               "Interchange Format. If you belive the file to be correct, please send the file to joshua@nettek.ca. "
               "Data Import for Ontario Tire Stewardship will now quit.");
    s_good=0;
}
