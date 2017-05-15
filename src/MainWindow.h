#pragma once

#include "Document.h"

#include <QMainWindow>
#include <QAction>
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
    void createDocumentActions();

private:
    struct
    {
        QAction *document_new;
        QAction *document_open;
        QAction *document_save;
        QAction *document_save_as;
        QAction *document_import;
        QAction *document_export;
    } actions;

    std::unique_ptr<Document> document;
};

