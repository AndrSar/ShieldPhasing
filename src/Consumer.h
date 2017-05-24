#pragma once

#include <QString>

class Shield;

class Consumer
{
public:
    Consumer();

private:
    QString name;
    unsigned int phase;
    Shield *shield;
};
