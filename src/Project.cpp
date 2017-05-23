#include "Project.h"

#include <QtSql>
#include <QFile>
#include <QDir>

#include <iostream>


std::unique_ptr<Project> Project::create(const QString &name, const QString &dir)
{
    QDir directory(dir);
    auto path = directory.filePath(name + ".db");
    if (QFile::exists(path))
        QFile::remove(path);

    auto db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);
    if (db.open())
    {
        return std::make_unique<Project>(name, dir, db);
    }
    else
    {
        return nullptr;
    }
}


Project::Project()
{

}


Project::Project(const QString &name, const QString &dir, QSqlDatabase db):
    name(name),
    dir(dir),
    db(db)
{
}


void Project::open(const QString &dbfile)
{

}


void Project::saveAs(const QString &path)
{

}


void Project::close()
{
    db.close();
}


QString Project::getName() const
{
    return name;
}


QString Project::getDir() const
{
    return dir;
}


QString Project::getPath() const
{
    return db.databaseName();
}
