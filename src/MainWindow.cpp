#include "MainWindow.h"

#include <QPalette>
#include <QMenuBar>
#include <QAction>
#include <QSettings>
#include <QLocale>
#include <QMessageBox>
#include <QFileDialog>
#include <QDialog>
#include <QTabWidget>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setMinimumSize(800, 500);
    createActions();
    createMenuBar();
    createOpenProjectFileDialog();
    createSaveAsProjectFileDialog();
    createConsumersDockWidget();
    createCentralTabWidget();
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
    menu->addAction(actions.project_new);
    menu->addAction(actions.project_open);
    menu->addAction(actions.project_save_as);
    menu->addAction(actions.project_close);
    menu->addSeparator();
    menu->addAction(actions.quit);
}


void MainWindow::createActions()
{
    createProjectActions();
    actions.quit = new QAction(tr("Quit"), this);
}


void MainWindow::updateActions()
{
    if (project != nullptr)
    {
        actions.project_save_as->setEnabled(true);
        actions.project_close->setEnabled(true);
    }
    else
    {
        actions.project_save_as->setEnabled(false);
        actions.project_close->setEnabled(false);
    }
}


void MainWindow::createProjectActions()
{
    actions.project_new = new QAction(tr("&New project..."), this);
    actions.project_open = new QAction(tr("&Open project..."), this);
    actions.project_save_as = new QAction(tr("Save as..."), this);
    actions.project_save_as->setEnabled(false);
    actions.project_close = new QAction(tr("Close"), this);
    actions.project_close->setEnabled(false);

    connect(actions.project_new, &QAction::triggered, this, &MainWindow::createNewProject);

    connect(actions.project_open, &QAction::triggered, [this](){
        fileDialogs.open_project->exec();
    });

    connect(actions.project_save_as, &QAction::triggered, [this](){
        fileDialogs.save_as_project->exec();
    });

    connect(actions.project_close, &QAction::triggered, this, &MainWindow::closeDocumentIfExists);
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


void MainWindow::createOpenProjectFileDialog()
{
    auto fileDialog = new QFileDialog(this);
    fileDialog->setFileMode(QFileDialog::FileMode::ExistingFile);
    fileDialog->setNameFilter("SQLite Database (*.db)");
    fileDialogs.open_project = fileDialog;
}


void MainWindow::createSaveAsProjectFileDialog()
{
    auto fileDialog = new QFileDialog(this);
    fileDialog->setFileMode(QFileDialog::FileMode::AnyFile);
    fileDialog->setNameFilter("SQLite Database (*.json)");
    fileDialog->setDefaultSuffix("db");
    fileDialog->setAcceptMode(QFileDialog::AcceptMode::AcceptSave);
    fileDialogs.save_as_project = fileDialog;
}


QString MainWindow::chooseDirectoryFromDialog()
{
//    QString res;
//    auto fileDialog = new QFileDialog(this, tr("Choose directory"));
//    fileDialog->setFileMode(QFileDialog::FileMode::Directory);
//    fileDialog->setAcceptMode(QFileDialog::AcceptMode::AcceptOpen);
//    fileDialog->setOption(QFileDialog::ShowDirsOnly, true);
//    if (fileDialog->exec())
//        res = fileDialog->sele;
//    return res;

    return QFileDialog::getExistingDirectory(this, tr("Choose directory"));
}


void MainWindow::createNewProject()
{
    closeDocumentIfExists();

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("New project settings"));
    dialog->setFixedSize(460, 200);

    QMargins textMargins(2, 0, 2, 1);

    QFormLayout *form = new QFormLayout();

    QLineEdit *nameEdit = new QLineEdit();
    nameEdit->setTextMargins(textMargins);
    QLineEdit *directoryEdit = new QLineEdit();
    directoryEdit->setTextMargins(textMargins);
    auto b = new QPushButton("...");
    b->setMaximumWidth(40);
    QHBoxLayout *hb = new QHBoxLayout();
    hb->addWidget(directoryEdit);
    hb->addWidget(b);

    form->addRow(tr("Project name"), nameEdit);
    form->addRow(tr("Project directory"), hb);

    form->setContentsMargins(QMargins(12, 12, 12, 12));
    form->setSpacing(8);
    dialog->setLayout(form);
    dialog->exec();

    project = std::make_unique<Project>();

    delete dialog;

    updateActions();
}


void MainWindow::closeDocumentIfExists()
{
    if (project == nullptr)
        return;

//    auto button = QMessageBox::warning(this, tr("Unsaved changes"),
//                               tr("Save changes in current document before continue?"),
//                               QMessageBox::StandardButtons( QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel),
//                               QMessageBox::Yes);

//    if (button == QMessageBox::Yes)
//    {
//        actions.document_save->trigger();
//    }

//    if (button != QMessageBox::Cancel)
//    {
//        project->close();
//        project = nullptr;
//    }

    project->close();
    project = nullptr;

    updateActions();
}


void MainWindow::createConsumersDockWidget()
{
    consumersDockWidget = new QDockWidget(tr("Consumers"), this);
    consumersDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    consumersDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea);

    this->addDockWidget(Qt::LeftDockWidgetArea, consumersDockWidget, Qt::Vertical);
}


void MainWindow::createCentralTabWidget()
{
    QTabWidget *tabWidget = new QTabWidget(this);
    tabWidget->setMinimumWidth(300);
    QWidget *t1 = new QWidget(tabWidget);
    QWidget *t2 = new QWidget(tabWidget);

    tabWidget->addTab(t1, tr("Phasing"));
    tabWidget->addTab(t2, tr("Cables"));
    this->setCentralWidget(tabWidget);
}


