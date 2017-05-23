#pragma once

#include "Project.h"

#include <QMainWindow>
#include <QAction>
#include <QFileDialog>
#include <QDockWidget>
#include <memory>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void createMenuBar();
    void createFileMenu();
    void createLanguageMenu();
    void createActions();
    void updateActions();
    void createProjectActions();
    void createOpenProjectFileDialog();
    void createSaveAsProjectFileDialog();
    QString chooseDirectoryFromDialog();
    void createNewProject();
    void closeDocumentIfExists();
    void createConsumersDockWidget();
    void createCentralTabWidget();
    //void createNewProjectDialog();

private:
    struct
    {
        QAction *project_new;
        QAction *project_open;
        QAction *project_save_as;
        QAction *project_close;
        //QAction *document_import;
        //QAction *document_export;
        QAction *quit;
    } actions;

    struct
    {
        QFileDialog *open_project;
        QFileDialog *save_as_project;
    } fileDialogs;

    QDockWidget *consumersDockWidget;

    std::unique_ptr<Project> project;
};

