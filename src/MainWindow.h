#pragma once

#include "Document.h"

#include <QMainWindow>
#include <QAction>
#include <QFileDialog>
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
    void createOpenDocumentFileDialog();
    void createSaveAsDocumentFileDialog();

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

    struct
    {
        QFileDialog *open_document;
        QFileDialog *save_as_document;
    } fileDialogs;

    std::unique_ptr<Document> document;
};

