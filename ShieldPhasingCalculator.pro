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
           src/Project.cpp \
           src/Workspace.cpp \
           src/models/Consumer.cpp \
           src/models/Shield.cpp \
           src/models/ShieldModel.cpp \
           src/models/ConsumerModel.cpp \
           src/models/TestModelsAccessProvider.cpp

HEADERS += src/MainWindow.h \
           src/Project.h \
           src/Workspace.h \
           src/models/Consumer.h \
           src/models/Shield.h \
           src/models/ShieldModel.h \
           src/models/ConsumerModel.h \
           src/models/ModelsAccessInterface.h \
           src/models/TestModelsAccessProvider.h


RESOURCES += translations.qrc

TRANSLATIONS = translations/en.ts \
               translations/ru.ts
