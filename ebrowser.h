/****************************************************
ebrowser.h

   Part of Data Import for Ontario Tire Stewardship
        Copyright (C) Joshua Netterfield 2012

                 All rights reserved.
*****************************************************/

#ifndef EMBEDDEDBROWSER_H
#define EMBEDDEDBROWSER_H

#include <QtGui/QWidget>
#include <QtWebKit/QWebView>
#include <QLabel>
#include <QStringList>
#include <QVariant>

class EBrowser : public QWidget
{
    Q_OBJECT

    QWebView* s_webView;
    QList<QWebElement> elements;
    QLabel* s_webPath,* s_question;

    QStringList getInputs();
public:
    EBrowser(QWidget *parent = 0);
    ~EBrowser();

    QString html() const;
    bool htmlContains(const QString& contains) const;

protected:
    void hideEvent(QHideEvent *)
    {
        emit visiblityChanged(0);
    }

    void showEvent(QShowEvent *)
    {
        emit visiblityChanged(1);
    }

public slots:
    void askQuestion(const QString& question);
    QVariant doJS(const QString& js);

    void loadPage(QString);

    //////////////////////////////////
    void loadFinishedLogic(bool s);
    void urlChangedLogic(QUrl url);

    void setInput(QString input,QString value);
    void clickInput(QString input);
signals:
    void ready();
    void visiblityChanged(bool);
};

#endif // EMBEDDEDBROWSER_H
