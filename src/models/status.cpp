#include "status.h"

Status::Status(QString uuid, QString name, QColor color)
{
    this->uuid = uuid;
    this->name = name;
    this->color = color;
}

const QString Status::getName()
{
    return this->name;
}

const QString Status::getUUID()
{
    return this->uuid;
}

const QColor Status::getColor()
{
    return this->color;
}
