#-------------------------------------------------
#
# Project created by QtCreator 2017-05-12T00:06:51
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PDFView
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp\
        mainwindow.cpp \
    abstractinfodock.cpp \
    documentobserver.cpp \
    embeddedfiles.cpp \
    fonts.cpp \
    info.cpp \
    metadata.cpp \
    navigationtoolbar.cpp \
    optcontent.cpp \
    pageview.cpp \
    permissions.cpp \
    thumbnails.cpp \
    connservice.cpp \
    logindialog.cpp \
    registerdialog.cpp \
    booklist.cpp \
    aes.cpp \
    workthread.cpp

HEADERS  += mainwindow.h \
    abstractinfodock.h \
    documentobserver.h \
    embeddedfiles.h \
    fonts.h \
    info.h \
    metadata.h \
    navigationtoolbar.h \
    optcontent.h \
    pageview.h \
    permissions.h \
    thumbnails.h \
    connservice.h \
    logindialog.h \
    registerdialog.h \
    booklist.h \
    aes.h \
    workthread.h

INCLUDEPATH += $$PWD/poppler

win32: LIBS += -L$$PWD/poppler -llibpoppler
win32: LIBS += -L$$PWD/poppler -llibpoppler-qt5
#INCLUDEPATH += $$PWD/crypto
#LIBS += -L$$PWD/crypto -lcryptopp
#LIBS += -L$$PWD/crypto -llibcryptopp

RESOURCES += \
    res.qrc

FORMS += \
    logindialog.ui \
    registerdialog.ui
