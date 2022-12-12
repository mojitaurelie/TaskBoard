#include "task.h"

Task::Task(QString title, QString description, QDate expectedFor, QString priorityUUID, QString statusUUID)
{
    this->title = title;
    this->description = description;
    this->expectedFor = expectedFor;
    this->priorityUUID = priorityUUID;
    this->statusUUID = statusUUID;
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

void Task::update(Task t)
{
    this->title = t.title;
    this->description = t.description;
    this->expectedFor = t.expectedFor;
    this->priorityUUID = t.priorityUUID;
    this->statusUUID = t.statusUUID;
}
