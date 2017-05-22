#pragma once

#include <QJsonDocument>
#include <QFile>
#include <QDateTime>


class Document
{
public:
    Document();
    Document(const QString &file);
    void save();
    void saveAs(const QString &path);
    bool isSavedToFile() const;
    bool hasUnsavedChanges() const;
    void close();
    void closeWithoutSaving();

private:
    QFile file;
    QJsonDocument jsonDocument;
    QDateTime lastModified;
    QDateTime lastSaved;
};
