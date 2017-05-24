#include "Workspace.h"


Workspace::Workspace(QMainWindow *mainWindow, std::weak_ptr<Project> &&project):
    mainWindow(mainWindow),
    project(project)
{
    createConsumersDockWidget();
    createShieldsDockWidget();
    createCentralTabWidget();
    createConsumersListView();
    createShieldsListView();
}


Workspace::~Workspace()
{
    delete consumersDockWidget;
    delete shieldsDockWidget;
    delete centralTabWidget;
}


void Workspace::close()
{

}


void Workspace::createConsumersDockWidget()
{
    consumersDockWidget = new QDockWidget(tr("Consumers"));
    consumersDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    consumersDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea);
    consumersDockWidget->setMinimumHeight(150);

    mainWindow->addDockWidget(Qt::LeftDockWidgetArea, consumersDockWidget);
}


void Workspace::createShieldsDockWidget()
{
    shieldsDockWidget = new QDockWidget(tr("Shields"));
    shieldsDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    shieldsDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea);
    shieldsDockWidget->setMinimumHeight(150);

    mainWindow->addDockWidget(Qt::LeftDockWidgetArea, shieldsDockWidget);
}


void Workspace::createCentralTabWidget()
{
    centralTabWidget = new QTabWidget();
    centralTabWidget->setMinimumWidth(300);
    QWidget *t1 = new QWidget(centralTabWidget);
    QWidget *t2 = new QWidget(centralTabWidget);

    centralTabWidget->addTab(t1, tr("Phasing"));
    centralTabWidget->addTab(t2, tr("Cables"));
    mainWindow->setCentralWidget(centralTabWidget);
}


void Workspace::createConsumersListView()
{
    consumersListView = new QListView();
    consumersDockWidget->setWidget(consumersListView);
}


void Workspace::createShieldsListView()
{
    shieldsListView = new QListView();
    shieldsDockWidget->setWidget(shieldsListView);
}
