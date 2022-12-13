#include "board.h"

Board::Board(QString name)
{
    this->name = name;
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

void Board::add(Task t)
{
    tasks.append(new Task(t));
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

