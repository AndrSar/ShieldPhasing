#pragma once

#include <QJsonDocument>
#include <QFile>


class Document
{
public:
    Document();
    Document(const QString &file);
    void save();
    void saveAs(const QString &path);
    bool isSavedToFile() const;

private:
    QFile file;
    QJsonDocument jsonDocument;
};
