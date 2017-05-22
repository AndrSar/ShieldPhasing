#pragma once

#include "Document.h"

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
    void createDocumentActions();
    void createOpenDocumentFileDialog();
    void createSaveAsDocumentFileDialog();
    void createNewDocument();
    void closeDocumentIfExists();
    void createConsumersDockWidget();

private:
    struct
    {
        QAction *document_new;
        QAction *document_open;
        QAction *document_save;
        QAction *document_save_as;
        QAction *document_close;
        QAction *document_import;
        QAction *document_export;
        QAction *quit;
    } actions;

    struct
    {
        QFileDialog *open_document;
        QFileDialog *save_as_document;
    } fileDialogs;

    QDockWidget *consumersDockWidget;

    std::unique_ptr<Document> document;
};

