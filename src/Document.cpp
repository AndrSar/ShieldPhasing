#include "Document.h"


Document::Document()
{

}


Document::Document(const QString &file)
{

}


void Document::save()
{

}


void Document::saveAs(const QString &path)
{

}


bool Document::isSavedToFile() const
{
    return file.isOpen();
}
