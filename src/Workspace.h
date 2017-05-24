#pragma once

#include "Project.h"
#include <QMainWindow>
#include <QWidget>
#include <QDockWidget>
#include <QTabWidget>
#include <QListView>

#include <memory>


class Workspace : public QObject
{
public:
    Workspace(QMainWindow *mainWindow, std::weak_ptr<Project> &&project);
    ~Workspace();

    void close();

private:
    void createConsumersDockWidget();
    void createShieldsDockWidget();
    void createCentralTabWidget();
    void createConsumersListView();
    void createShieldsListView();

private:
    QMainWindow *mainWindow;
    std::weak_ptr<Project> project;

    QDockWidget *consumersDockWidget;
    QDockWidget *shieldsDockWidget;
    QTabWidget *centralTabWidget;

    QListView *consumersListView;
    QListView *shieldsListView;
};
