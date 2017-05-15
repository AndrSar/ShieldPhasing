#include "MainWindow.h"
#include <QApplication>
#include <QLibraryInfo>
#include <QTranslator>
#include <QSettings>
#include <QLocale>


#include <iostream>


QLocale getLocaleFromLanguageSettings(const QString &language)
{
    QLocale locale;
    if (language.isEmpty())
        locale = QLocale::system();
    else
        locale = QLocale(language);

    return locale;
}


int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    application.setOrganizationName("Andrey Lifanov");
    application.setApplicationName("ShieldPhasingCalculator");


    QSettings settings;
    const QString languageOption = settings.value("language", QString()).toString();
    QLocale locale = getLocaleFromLanguageSettings(languageOption);
    QLocale::setDefault(locale);


    // Load translators
    //QTranslator qtTranslator;
    //if (qtTranslator.load("qt_ru", QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
    //{
    //    application.installTranslator(&qtTranslator);
    //}


    QTranslator translator;
    std::cout << translator.load("translation.qm", ":/") << std::endl;
    application.installTranslator(&translator);

    application.setApplicationDisplayName(application.tr("Shield Phasing"));

    MainWindow mainWindow;
    mainWindow.show();

    return application.exec();
}
