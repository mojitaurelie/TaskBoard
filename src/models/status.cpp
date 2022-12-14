#include "status.h"

#define UUID_KEY "uuid"
#define NAME_KEY "name"
#define COLOR_KEY "color"

#include <QUuid>

Status::Status(QString uuid, QString name, QColor color)
{
    this->uuid = uuid;
    this->name = name;
    this->color = color;
}

Status::Status(QJsonObject obj)
{
    QUuid uuid = QUuid::createUuid();
    this->uuid = obj[UUID_KEY].toString(uuid.toString(QUuid::WithoutBraces));
    this->name = obj[NAME_KEY].toString("!Missing name!");
    this->color = QColor(obj[COLOR_KEY].toString("#ffffff"));
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

const QJsonObject Status::toJson()
{
    QJsonObject obj;
    obj[UUID_KEY] = this->uuid;
    obj[NAME_KEY] = this->name;
    obj[COLOR_KEY] = this->color.name(QColor::HexRgb);
    return obj;
}
