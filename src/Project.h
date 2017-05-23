#pragma once

#include <QFile>
#include <QDateTime>


class Project
{
public:
    Project();
    Project(const QString &dbfile);
    void saveAs(const QString &path);
    void close();

private:
    QFile file;
    QDateTime lastModified;
    QDateTime lastSaved;
};
