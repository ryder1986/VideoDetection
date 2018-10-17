#-------------------------------------------------
#
# Project created by QtCreator 2018-01-19T09:37:39
#
#-------------------------------------------------
# Qt shared
QT_CI_JENKINS_HOME=$$(JENKINS_HOME)
!isEmpty(QT_CI_JENKINS_HOME) {
    message("Qt CI environment detected, suppressing example registration")
    CONFIG += qaxserver_no_postlink
}

TEMPLATE = app
TARGET   = ActiveX
CONFIG += warn_off
QT += core widgets axserver
SOURCES += \
        main.cpp \
        activex.cpp \

HEADERS += \
        activex.h

FORMS += \
        activex.ui

RC_FILE  = ActiveX.rc
DEF_FILE = ActiveX.def
