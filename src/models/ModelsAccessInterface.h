#pragma once

#include <QString>
#include "Shield.h"
#include "Consumer.h"
#include <vector>


class ModelsAccessInterface
{
public:
    virtual void createConsumer(const QString &name) = 0;
    virtual void createShield(const QString &name) = 0;
    virtual std::vector<Shield> getAllShields() = 0;
};
