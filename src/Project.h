#pragma once

#include "models/ModelsAccessInterface.h"
#include "models/TestModelsAccessProvider.h"

#include <QFile>
#include <QDateTime>
#include <QSqlDatabase>
#include <memory>


class Project
{
public:
    static std::unique_ptr<Project> create(const QString &name, const QString &dir);
    static std::unique_ptr<Project> open(const QString &path);

public:
    Project();
    Project(const QString &name, const QString &dir, QSqlDatabase db);
    void saveAs(const QString &path);
    void close();
    QString getName() const;
    QString getDir() const;
    QString getPath() const;

private:
    QString name;
    QString dir;
    QSqlDatabase db;
    QDateTime lastModified;
    QDateTime lastSaved;

private:
    std::shared_ptr<ModelsAccessInterface> modelsAccessInterface;
};
