#include "priority.h"

Priority::Priority(QString uuid, QString name, QColor color)
{
    this->uuid = uuid;
    this->name = name;
    this->color = color;
}

const QString Priority::getName()
{
    return this->name;
}

const QString Priority::getUUID()
{
    return this->uuid;
}

const QColor Priority::getColor()
{
    return this->color;
}
