#pragma once

#include <QString>


class Shield
{
public:
    Shield(const QString name);

private:
    QString name;
    unsigned int power_capacity;
    int floor;
};
