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
    createConsumersDockWidget();
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
    menu->addAction(actions.document_close);
    menu->addSeparator();
    menu->addAction(actions.quit);
}


void MainWindow::createActions()
{
    createDocumentActions();
    actions.quit = new QAction(tr("Quit"), this);
}


void MainWindow::updateActions()
{
    if (document != nullptr)
    {
        actions.document_save->setEnabled(true);
        actions.document_save_as->setEnabled(true);
        actions.document_close->setEnabled(true);
    }
    else
    {
        actions.document_save->setEnabled(false);
        actions.document_save_as->setEnabled(false);
        actions.document_close->setEnabled(false);
    }
}


void MainWindow::createDocumentActions()
{
    actions.document_new = new QAction(tr("New"), this);
    actions.document_open = new QAction(tr("Open..."), this);
    actions.document_save = new QAction(tr("Save"), this);
    actions.document_save->setEnabled(false);
    actions.document_save_as = new QAction(tr("Save as..."), this);
    actions.document_save_as->setEnabled(false);
    actions.document_close = new QAction(tr("Close"), this);
    actions.document_close->setEnabled(false);

    connect(actions.document_new, &QAction::triggered, this, &MainWindow::createNewDocument);

    connect(actions.document_open, &QAction::triggered, [this](){
        fileDialogs.open_document->exec();
    });

    connect(actions.document_save_as, &QAction::triggered, [this](){
        fileDialogs.save_as_document->exec();
    });

    connect(actions.document_save, &QAction::triggered, [this](){
        if (this->document != nullptr && this->document->isSavedToFile())
        {
            this->document->save();
        }
        else
        {
            actions.document_save_as->trigger();
        }
    });

    connect(actions.document_close, &QAction::triggered, this, &MainWindow::closeDocumentIfExists);
}


void MainWindow::createLanguageMenu()
{
    QMenu *menu = this->menuBar()->addMenu(tr("&Language"));
    auto actionGroup = new QActionGroup(menu);
    actionGroup->setExclusive(true);

    auto create_language_action = [menu, actionGroup](QLocale locale)
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


void MainWindow::createNewDocument()
{
    closeDocumentIfExists();

    if (document == nullptr)
    {
        document = std::make_unique<Document>();
    }

    updateActions();
}


void MainWindow::closeDocumentIfExists()
{
    if (document == nullptr)
        return;

    auto button = QMessageBox::warning(this, tr("Unsaved changes"),
                               tr("Save changes in current document before continue?"),
                               QMessageBox::StandardButtons( QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel),
                               QMessageBox::Yes);

    if (button == QMessageBox::Yes)
    {
        actions.document_save->trigger();
    }

    if (button != QMessageBox::Cancel)
    {
        document->close();
        document = nullptr;
    }

    updateActions();
}


void MainWindow::createConsumersDockWidget()
{
    consumersDockWidget = new QDockWidget(tr("Consumers"), this);
    consumersDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    consumersDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea);
    consumersDockWidget->setWidth(300);
    this->addDockWidget(Qt::LeftDockWidgetArea, consumersDockWidget, Qt::Vertical);
}


