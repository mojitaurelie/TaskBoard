#ifndef BOARD_H
#define BOARD_H

#include <QVector>
#include <QString>

#include "task.h"

class Board
{
public:
    Board(QString name);
    ~Board();

    const QString getName();
    void add(Task);
    Task *taskAt(int);
    const QVector<Task*> getTasks();

private:
    QVector<Task*> tasks;
    QString name;

};

#endif // BOARD_H
