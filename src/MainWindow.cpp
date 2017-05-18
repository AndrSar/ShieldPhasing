#include "MainWindow.h"

#include <QLayout>
#include <QMenuBar>
#include <QAction>
#include <QSettings>
#include <QLocale>
#include <QMessageBox>
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setMinimumSize(800, 500);
    createActions();
    createMenuBar();
    createOpenDocumentFileDialog();
    createSaveAsDocumentFileDialog();
}


MainWindow::~MainWindow()
{

}


void MainWindow::createMenuBar()
{
    QMenuBar &menuBar = *this->menuBar();
    createFileMenu();
    auto consumersMenu = menuBar.addMenu(tr("&Consumers"));
    createLanguageMenu();
}


void MainWindow::createFileMenu()
{
    QMenu *menu = this->menuBar()->addMenu(tr("&File"));
    menu->addAction(actions.document_new);
    menu->addAction(actions.document_open);
    menu->addAction(actions.document_save_as);
    menu->addAction(actions.document_save);
}


void MainWindow::createActions()
{
    createDocumentActions();
}


void MainWindow::createDocumentActions()
{
    actions.document_new = new QAction(tr("New"), this);
    actions.document_open = new QAction(tr("Open..."), this);
    actions.document_save = new QAction(tr("Save"), this);
    actions.document_save_as = new QAction(tr("Save as..."), this);

    connect(actions.document_open, &QAction::triggered, [this](){
        fileDialogs.open_document->exec();
    });

    connect(actions.document_save_as, &QAction::triggered, [this](){
        fileDialogs.save_as_document->exec();
    });
}


void MainWindow::createLanguageMenu()
{
    QMenu *menu = this->menuBar()->addMenu(tr("&Language"));
    auto actionGroup = new QActionGroup(menu);
    actionGroup->setExclusive(true);

    auto create_language_action = [menu, actionGroup](QLocale &locale)
    {
        auto action = menu->addAction(locale.nativeLanguageName());
        action->setCheckable(true);
        actionGroup->addAction(action);

        connect(action, &QAction::triggered, [locale]() {
            QSettings settings;
            settings.setValue("language", locale.name());
        });

        QLocale currentLocale;
        if (currentLocale.language() == locale.language())
        {
            action->setChecked(true);
        }
    };

    create_language_action(QLocale(QLocale::English));
    create_language_action(QLocale(QLocale::Russian));

    connect(actionGroup, &QActionGroup::triggered, [this]() {
        QMessageBox::information(this, tr("Language switching"),
                                 tr("Restart application to switch language"));
    });
}


void MainWindow::createOpenDocumentFileDialog()
{
    auto fileDialog = new QFileDialog(this);
    fileDialog->setFileMode(QFileDialog::FileMode::ExistingFile);
    fileDialog->setNameFilter("JSON document (*.json)");
    fileDialogs.open_document = fileDialog;
}


void MainWindow::createSaveAsDocumentFileDialog()
{
    auto fileDialog = new QFileDialog(this);
    fileDialog->setFileMode(QFileDialog::FileMode::AnyFile);
    fileDialog->setNameFilter("JSON document (*.json)");
    fileDialog->setDefaultSuffix("json");
    fileDialog->setAcceptMode(QFileDialog::AcceptMode::AcceptSave);
    fileDialogs.save_as_document = fileDialog;
}

