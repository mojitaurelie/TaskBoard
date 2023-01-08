#include "priority.h"

#define UUID_KEY "uuid"
#define NAME_KEY "name"
#define COLOR_KEY "color"

#include <QUuid>

Priority::Priority(QString uuid, QString name, QColor color)
{
    this->uuid = uuid;
    this->name = name;
    this->color = color;
}

Priority::Priority(QJsonObject obj)
{
    QUuid uuid = QUuid::createUuid();
    this->uuid = obj[UUID_KEY].toString(uuid.toString(QUuid::WithoutBraces));
    this->name = obj[NAME_KEY].toString("!Missing name!");
    this->color = QColor(obj[COLOR_KEY].toString("#ffffff"));
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

const QJsonObject Priority::toJson()
{
    QJsonObject obj;
    obj[UUID_KEY] = this->uuid;
    obj[NAME_KEY] = this->name;
    obj[COLOR_KEY] = this->color.name(QColor::HexRgb);
    return obj;
}
