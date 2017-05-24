#pragma once

#include "Consumer.h"
#include <QString>
#include <vector>


class Shield
{
public:
    Shield(const QString name, unsigned int power_capacity);

private:
    QString name;
    unsigned int power_capacity;
    std::vector<Consumer> consumers;
};
