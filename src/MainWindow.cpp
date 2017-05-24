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
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QValidator>
#include <QDir>
#include <QLabel>
#include <QPalette>
#include <QFile>
#include <QCloseEvent>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //setWindowTitle(baseWindowTitle());
    setMinimumSize(800, 500);
    createActions();
    createMenuBar();
    createOpenProjectFileDialog();
    createSaveAsProjectFileDialog();
}


MainWindow::~MainWindow()
{

}


QString MainWindow::baseWindowTitle() const
{
    return tr("Shield Phasing");
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
    actions.quit->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q));
    connect(actions.quit, &QAction::triggered, this, &QWidget::close);
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
    connect(actions.project_open, &QAction::triggered, this, &MainWindow::openProject);

    connect(actions.project_save_as, &QAction::triggered, [this](){
        fileDialogs.save_as_project->exec();
    });

    connect(actions.project_close, &QAction::triggered, this, &MainWindow::closeProjectIfExists);

    actions.project_new->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
    actions.project_open->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
    actions.project_save_as->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
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
    fileDialog->setNameFilter("SQLite Database (*.db)");
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
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("New project settings"));
    dialog->setFixedSize(460, 200);

    QMargins textMargins(2, 0, 2, 1);

    auto layout = new QVBoxLayout();
    auto form = new QFormLayout();
    auto dialogButtons = new QHBoxLayout();
    layout->addLayout(form);
    layout->addLayout(dialogButtons);

    QLineEdit *nameEdit = new QLineEdit();
    nameEdit->setTextMargins(textMargins);
    QLineEdit *directoryEdit = new QLineEdit();
    directoryEdit->setTextMargins(textMargins);
    QLabel *resultedFilePath = new QLabel();
    auto directoryChooserButton = new QPushButton("...");
    directoryChooserButton->setMaximumWidth(40);
    QHBoxLayout *hb = new QHBoxLayout();
    hb->addWidget(directoryEdit);
    hb->addWidget(directoryChooserButton);

    form->addRow(tr("Project name"), nameEdit);
    form->addRow(tr("Project directory"), hb);
    form->addRow(tr("Path:"), resultedFilePath);

    form->setContentsMargins(QMargins(12, 12, 12, 12));
    form->setSpacing(8);
    dialog->setLayout(layout);

    auto createButton = new QPushButton(tr("Create"));
    createButton->setEnabled(false);
    auto cancelButton = new QPushButton(tr("Cancel"));
    dialogButtons->addSpacing(dialog->width()/2);
    dialogButtons->addWidget(cancelButton);
    dialogButtons->addWidget(createButton);

    // Behaviour and logic
    connect(createButton, &QPushButton::released, dialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::released, dialog, &QDialog::reject);
    connect(directoryChooserButton, &QPushButton::released, [dialog, directoryEdit](){
        QString str = QFileDialog::getExistingDirectory(dialog, tr("Choose directory"));
        if (!str.isEmpty())
            directoryEdit->setText(str);
    });

    auto formChecker = [nameEdit, directoryEdit, createButton, resultedFilePath](const QString &text){
        if (!nameEdit->text().isEmpty() && !directoryEdit->text().isEmpty())
        {
            QDir dir(directoryEdit->text());
            QString filePath = dir.filePath(nameEdit->text() + ".db");
            resultedFilePath->setText(filePath);
            QPalette palette;
            palette.setColor(QPalette::Foreground, QColor("red"));
            if (dir.exists())
            {
                createButton->setEnabled(true);
                palette.setColor(QPalette::Foreground, QColor("black"));
            }
            else
            {
                createButton->setEnabled(false);
            }

            resultedFilePath->setPalette(palette);
        }
        else
        {
            resultedFilePath->setText("");
            createButton->setEnabled(false);
        }
    };

    connect(nameEdit, &QLineEdit::textChanged, formChecker);
    connect(directoryEdit, &QLineEdit::textChanged, formChecker);

    if (dialog->exec() == QDialog::Accepted)
    {
        QString filePath = resultedFilePath->text();
        bool canProceed = true;
        if (QFile::exists(filePath))
        {
            auto overwrite = QMessageBox::warning(dialog, tr("Overwrite file"),
                                                  tr("File already exists. Proceed to ovewrite?"),
                                                  QMessageBox::Cancel | QMessageBox::Ok,
                                                  QMessageBox::Ok);
            canProceed = overwrite == QMessageBox::Ok;
        }

        if (canProceed)
        {
            closeProjectIfExists();
            project = Project::create(nameEdit->text(), directoryEdit->text());
            if (project == nullptr)
            {
                QMessageBox::critical(dialog, tr("Error"),
                                              tr("Couldn't create a project"));
            }
            else
            {
                initProjectWorkspace();
            }
        }
    }

    delete dialog;
    updateActions();
}


void MainWindow::openProject()
{
    connect(fileDialogs.open_project, &QFileDialog::fileSelected, [this](const QString &file)
    {
        closeProjectIfExists();

        project = Project::open(file);
        if (project == nullptr)
        {
            QMessageBox::critical(this, tr("Error"),
                                          tr("Couldn't open a project"));
        }
        else
        {
            updateActions();
            initProjectWorkspace();
        }
    });

    fileDialogs.open_project->exec();
}


void MainWindow::closeProjectIfExists()
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

    closeProjectWorkspace();

    project->close();
    project = nullptr;

    updateActions();
    updateTitle();
}


void MainWindow::initProjectWorkspace()
{
    workspace = std::make_unique<Workspace>(this, project);
    updateTitle();
}


void MainWindow::closeProjectWorkspace()
{
    if (workspace)
    {
        workspace->close();
        workspace = nullptr;
    }
}


void MainWindow::updateTitle()
{
    if (project)
    {
        setWindowTitle(project->getPath());
    }
    else
    {
        setWindowTitle("");
    }
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    closeProjectIfExists();
    event->accept();
}


