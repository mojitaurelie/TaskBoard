#ifndef BOARD_H
#define BOARD_H

#include <QVector>
#include <QString>
#include <QJsonObject>

#include "task.h"

class Board
{
public:
    Board(QString name);
    Board(QJsonObject);
    ~Board();

    const QString getName();
    void setName(const QString name);
    void add(Task);
    void remove(uint16_t index);
    Task *taskAt(uint16_t);
    const QVector<Task*> getTasks();

    const QJsonObject toJson();

private:
    QVector<Task*> tasks;
    QString name;

};

#endif // BOARD_H
