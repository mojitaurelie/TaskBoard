#ifndef FILTER_H
#define FILTER_H

#include <QVector>
#include <QString>
#include <QJsonObject>

#include "task.h"
#include "board.h"

class filter
{
private:
    QString name;
    // 0 none, 1 lower, 2 equal, 3 upper
    uint8_t expectedForComparator;
    // if empty => all
    QVector<QString> boards;
    QVector<QString> status;
    QVector<QString> priorities;
public:
    filter(QString name, uint8 expectedForComparator,QVector<QString> boards,QVector<QString> status,QVector<QString> priorities);
    filter(QJsonObject obj);

    QVector<Task*> get(QVector<Board*>);

    bool filterStatus(Task *t);
    bool filterPriority(Task *t);
    bool filterDate(Task *t, QDate now);
    const QJsonObject toJson();
};

#endif // FILTER_H
