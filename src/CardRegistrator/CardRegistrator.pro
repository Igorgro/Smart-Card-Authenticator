#-------------------------------------------------
#
# Project created by QtCreator 2019-02-20T14:40:37
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = CardRegistrator
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    addcarddialog.cpp \
    cardreaderdialog.cpp \
    helpers.cpp \
    settings.cpp \
    settingsdialog.cpp \
    credential.cpp \
    credentialsstorage.cpp \
    editcardsdialog.cpp \
    messagedialog.cpp

HEADERS += \
        mainwindow.h \
    addcarddialog.h \
    cardreaderdialog.h \
    helpers.h \
    settings.h \
    settingsdialog.h \
    credential.h \
    credentialsstorage.h \
    editcardsdialog.h \
    messagedialog.h

FORMS += \
        mainwindow.ui \
    addcarddialog.ui \
    cardreaderdialog.ui \
    settingsdialog.ui \
    editcarddialog.ui \
    messagedialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
RC_FILE=cardregistrator.rc

win32: LIBS += -L$$PWD/lib/ -lsqlite3 -lCardReader

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include


win32: LIBS += -L'C:/Program Files (x86)/Windows Kits/10/Lib/10.0.17134.0/um/x64/' -lWinSpool
