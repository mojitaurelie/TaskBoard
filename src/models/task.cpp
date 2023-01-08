#include "task.h"

#define TITLE_KEY "title"
#define DESCRIPTION_KEY "description"
#define EXPECTEDFOR_KEY "expected_for"
#define PRIORITY_KEY "priority"
#define STATUS_KEY "status"
#define UUID_KEY "uuid"

#include <QUuid>

Task::Task(QString title, QString description, QDate expectedFor, QString priorityUUID, QString statusUUID)
{
    QUuid uuid = QUuid::createUuid();
    this->uuid = uuid.toString(QUuid::WithoutBraces);
    this->title = title;
    this->description = description;
    this->expectedFor = expectedFor;
    this->priorityUUID = priorityUUID;
    this->statusUUID = statusUUID;
}

Task::Task(QJsonObject obj)
{
    QUuid uuid = QUuid::createUuid();
    this->uuid = obj[UUID_KEY].toString(uuid.toString(QUuid::WithoutBraces));
    this->title = obj[TITLE_KEY].toString();
    this->description = obj[DESCRIPTION_KEY].toString();
    this->expectedFor = QDate::fromString(obj[EXPECTEDFOR_KEY].toString(), Qt::DateFormat::ISODate);
    this->priorityUUID = obj[PRIORITY_KEY].toString();
    this->statusUUID = obj[STATUS_KEY].toString();
}

const QString Task::getUuid()
{
    return this->uuid;
}

const QString Task::getTitle()
{
    return this->title;
}

const QString Task::getDescription()
{
    return this->description;
}

const QDate Task::getExpectedFor()
{
    return this->expectedFor;
}

const QString Task::getPriorityUUID()
{
    return this->priorityUUID;
}

const QString Task::getStatusUUID()
{
    return this->statusUUID;
}

const QJsonObject Task::toJson()
{
    QJsonObject obj;
    obj[TITLE_KEY] = this->title;
    obj[DESCRIPTION_KEY] = this->description;
    obj[EXPECTEDFOR_KEY] = this->expectedFor.toString(Qt::DateFormat::ISODate);
    obj[PRIORITY_KEY] = this->priorityUUID;
    obj[STATUS_KEY] = this->statusUUID;
    return obj;
}

void Task::update(Task t)
{
    this->title = t.title;
    this->description = t.description;
    this->expectedFor = t.expectedFor;
    this->priorityUUID = t.priorityUUID;
    this->statusUUID = t.statusUUID;
}
