/****************************************************
edifreader.h

   Part of Data Import for Ontario Tire Stewardship
        Copyright (C) Joshua Netterfield 2012

                 All rights reserved.
*****************************************************/

#ifndef EDIFREADER_H
#define EDIFREADER_H

#include <QString>
#include <QStringList>
#include <QFile>
#include <QList>
#include <QObject>
#include <QDebug>

class EDifReader : public QObject
{
    Q_OBJECT

    QString s_path;
    QFile s_file;
    bool s_good;
    int s_columns,s_rows;
    void notifyInvalid();
    QList< QStringList > s_data;  //i.e., data[row][col]
public:
    EDifReader(const QString& path);
    void init();    // must be called before data can be accessed.
    const int& columnCount() const { return s_columns; }
    const int& rowCount() const { return s_rows; }
    const QString& getData(const int& column, const int& row) const
    {
        Q_ASSERT(column>=0);
        Q_ASSERT(row>=0);
        Q_ASSERT(column<s_columns);
        Q_ASSERT(row<s_rows);
        // ^ is it initialized? if not, rows will be columns will be -1
        qDebug()<<"ROW:"<<row<<"/"<<s_data.size()<<"%%";
        qDebug()<<"COL:"<<column<<"/"<<s_data[row].size()<<"%%";
        return s_data[row][column];
    }
signals:
    void error(QString err);
};

#endif // EDIFREADER_H
