#pragma once

#include <QString>


class Shield
{
public:
    Shield(const QString name, unsigned int power_capacity);

private:
    QString name;
    unsigned int power_capacity;
};
