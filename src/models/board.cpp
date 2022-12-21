#include "board.h"

#define NAME_KEY "name"
#define TASKS_KEY "tasks"

#include <QJsonArray>
#include <QJsonValue>

Board::Board(QString name)
{
    this->name = name;
}

Board::Board(QJsonObject obj)
{
    this->name = obj[NAME_KEY].toString();
    QJsonArray jsonTasks = obj[TASKS_KEY].toArray();
    for (QJsonValue value : jsonTasks) {
        Task *t = new Task(value.toObject());
        this->tasks.append(t);
    }
}

Board::~Board()
{
    for (uint16_t i = 0; i < tasks.count(); i++)
    {
        Task *t = tasks.takeAt(i);
        delete t;
    }
}

const QString Board::getName()
{
    return name;
}

void Board::setName(const QString name)
{
    this->name = name;
}

void Board::add(Task t)
{
    tasks.append(new Task(t));
}

void Board::remove(uint16_t index)
{
    if (index < tasks.count())
    {
        tasks.removeAt(index);
    }
}

Task *Board::taskAt(uint16_t i)
{
    if (i < tasks.count())
    {
        return tasks[i];
    }
    return nullptr;
}

const QVector<Task *> Board::getTasks()
{
    return tasks;
}

const QJsonObject Board::toJson()
{
    QJsonArray array;
    foreach (Task *t, this->tasks) {
        array.append(t->toJson());
    }
    QJsonObject obj;
    obj[NAME_KEY] = this->name;
    obj[TASKS_KEY] = array;
    return obj;
}

