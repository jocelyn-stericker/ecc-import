/****************************************************
edifaction.cpp

   Part of Data Import for Ontario Tire Stewardship
        Copyright (C) Joshua Netterfield 2012

                 All rights reserved.
*****************************************************/

#include "edifaction.h"
#include <QMessageBox>

EDIFAction::EDIFAction(EBrowser &browser, EDifReader &reader) : EAction(browser), s_reader(reader), s_curRow(0), s_curCol(0), s_rowBlank(1)
{
    for(int i=0;i<s_reader.columnCount();i++)
    {
        s_colActions.push_back("");
    }
}

void EDIFAction::continueAction(bool strict)
{
    if (!strict) {
        RUN_SECOND_TIME
    }

    // quick/dirty.

    disconnect(&s_browser,SIGNAL(ready()),this,SLOT(continueAction())); //may get warning. who cares?

    bool breakNow=0;

    while(1)
    {
        QString procStr=s_reader.getData(s_curCol,s_curRow);
        if(procStr.indexOf("{")<procStr.lastIndexOf("}"))
        {
            procStr.remove(0,procStr.indexOf("{")+1);
            procStr.truncate(procStr.lastIndexOf("}"));
            if(procStr.contains('+'))
            {
                procStr.remove('+');
                s_colActions[s_curCol]=procStr;
            }
            bool doAction=1;
            if(procStr.contains("IF !ROWBLANK,"))
            {
                procStr.remove("IF !ROWBLANK,");
                doAction=doAction?!s_rowBlank:0;
            }
            while(procStr.startsWith(' '))
            {
                procStr.remove(0,1);
            }
            while(procStr.endsWith(' ')) {
                procStr.chop(1);
            }
            if(doAction)
            {
                if(procStr.contains("CLICK,"))
                {
                    procStr.remove("CLICK,");
                    s_browser.clickInput(procStr);
                    breakNow=1;
                    connect(&s_browser,SIGNAL(ready()),this,SLOT(continueAction()));
                }
                else if(procStr.contains("GOTO,"))
                {
                    procStr.remove("GOTO,");
                    s_browser.loadPage(procStr);
                    breakNow=1;
                    connect(&s_browser,SIGNAL(ready()),this,SLOT(continueAction()));
                }
            }
        }
        else if(s_colActions[s_curCol]!="")
        {
            if(s_colActions[s_curCol].startsWith("AUTODATE")&&procStr!="")
            {
                s_colActions[s_curCol].remove("AUTODATE");
                QString map[12]={"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};
                procStr.replace("ERR: ", "");
                procStr.replace(" (V)", "");
                procStr.replace("#", "");
                QStringList args=procStr.split('-');
                if(args.size()!=3)
                {
                    emit error("Malformed date string. Got " + procStr + ". Syntax is DD-MMM-YY. Data may have been submitted. "
                               "Please review file at www.rethinktires.ca");
                    return;
                }
                int month=-1;
                for(int i=0;i<12;i++)
                {
                    if(args[1].toUpper()==map[i])
                    {
                        month=i+1;
                        break;
                    }
                }
                if(month==-1)
                {
                    emit error("Malformed date string: Syntax is DD-MMM-YY. MMM must be one of JAN,FEB,MAR,APR,MAY,"
                               "JUN,JUL,AUG,SEP,OCT,NOV OR DEC (case insensitive). Data may have been submitted. Please review file at "
                               "www.rethinktires.ca");
                    return;
                }
                while(args.at(0).startsWith("0"))
                {
                    args[0].remove(0,1);
                }
                s_browser.setInput(s_colActions[s_curCol]+"day",args[0]);
                s_browser.setInput(s_colActions[s_curCol]+"month",QString::number(month));
                int a = args[2].toInt();
                if (a < 2000) a += 2000;
                args[2] = QString::number(a);

                s_browser.setInput(s_colActions[s_curCol]+"year",args[2]);
                qDebug()<<"SETTING"<<s_colActions[s_curCol]+"day"<<"TO"<<args[0];
                s_colActions[s_curCol]="AUTODATE"+s_colActions[s_curCol];
            }
            else if(procStr!="")
            {
                if(procStr=="N")
                {
                    procStr="0";
                }
                else if(procStr=="Y")
                {
                    procStr="1";
                }
                while(procStr.endsWith(' '))
                {
                    procStr.chop(1);
                }
                while(procStr.startsWith(' '))
                {
                    procStr.remove(0,1);
                }
                s_browser.setInput(s_colActions[s_curCol],procStr);
            }
        }

        if(procStr!="")
        {
            s_rowBlank=0;
        }

        ++s_curCol;
        if(s_curCol==s_reader.columnCount())
        {
            s_curCol=0;
            s_rowBlank=1;
            ++s_curRow;
            emit info("Row "+QString::number(s_curRow)+" of "+QString::number(s_reader.rowCount()));
        }
        if(s_curRow>=s_reader.rowCount())
        {
            emit done();
            break;
        }

        if(breakNow)
        {
            break;
        }
    }
}
