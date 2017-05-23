#-------------------------------------------------
#
# Project created by QtCreator 2017-05-13T19:14:56
#
#-------------------------------------------------

QT       += core gui sql
CONFIG   += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ShieldPhasingCalculator
TEMPLATE = app


SOURCES += src/main.cpp    \
           src/MainWindow.cpp \
           src/Project.cpp

HEADERS  += src/MainWindow.h \
            src/Project.h

RESOURCES += translations.qrc

TRANSLATIONS = translations/en.ts \
               translations/ru.ts
