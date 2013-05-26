#-------------------------------------------------
#
# Project created by QtCreator 2012-10-19T20:30:45
#
#-------------------------------------------------

QT       += core gui network webkit

TARGET = eccImport
TEMPLATE = app


SOURCES += main.cpp \
    ewizardpage.cpp \
    ebrowser.cpp \
    ewizard.cpp \
    eaction.cpp \
    eloginaction.cpp \
    edifreader.cpp \
    eeula.cpp \
    ehaulervalidateaction.cpp \
    ehelpdialog.cpp \
    eselectclaimaction.cpp \
    edifaction.cpp \
    enewclaimaction.cpp \
    edeleteclaimsaction.cpp \
    ecolllogininfoaction.cpp \
    ecollaction.cpp

HEADERS  += \
    ewizardpage.h \
    ebrowser.h \
    ewizard.h \
    eaction.h \
    eloginaction.h \
    edifreader.h \
    eeula.h \
    ehaulervalidateaction.h \
    ehelpdialog.h \
    eselectclaimaction.h \
    edifaction.h \
    enewclaimaction.h \
    edeleteclaimsaction.h \
    ecolllogininfoaction.h \
    ecollaction.h

FORMS    += \
    demowizard.ui \
    eeula.ui \
    ehelpdialog.ui

RESOURCES += \
    rc.qrc

RC_FILE = eccImport.rc






