#include "filter.h"

#define NAME_KEY "name"
#define DATE_KEY "date"
#define BOARDS_KEY "boards"
#define STATUS_KEY "status"
#define PRIORITY_KEY "priority"

filter::filter(QString name, uint8 expectedForComparator, QVector<QString> boards, QVector<QString> status, QVector<QString> priorities)
{
    this->name = name;
    this->expectedForComparator = expectedForComparator;
    this->boards = boards;
    this->status = status;
    this->priorities = priorities;
}

filter::filter(QJsonObject obj)
{
    this->name = obj[NAME_KEY].toString("!Missing name!");
    this->expectedForComparator = obj[DATE_KEY].toInt();
    QJsonArray b = obj[BOARDS_KEY].toArray();
    QJsonArray s = obj[STATUS_KEY].toArray();
    QJsonArray p = obj[PRIORITY_KEY].toArray();
    foreach (QJsonValue value, b)
    {
        if (value.isString())
        {
            boards.append(value.toString());
        }
    }
    foreach (QJsonValue value, s)
    {
        if (value.isString())
        {
            status.append(value.toString());
        }
    }
    foreach (QJsonValue value, p)
    {
        if (value.isString())
        {
            priorities.append(value.toString());
        }
    }
}

QVector<Task *> filter::get(QVector<Board *> allBoards)
{
    QDate now = QDate::currentDate();
    QVector<Board*> selectedBoards;
    if (this->boards.count() > 0)
    {
        for (Board *b : allBoards)
        {
            foreach (QString boardUuid, this->boards)
            {
                if (b->getUuid() == boardUuid)
                {
                    selectedBoards.append(b);
                }
            }
        }
    }
    else
    {
        selectedBoards = allBoards;
    }

    bool fstatus, fpriority, fdate;
    QVector<Task*> result;
    for (Board *b : selectedBoards)
    {
        for (Task *t : b->getTasks())
        {
            fstatus = filterStatus(t);
            fpriority = filterPriority(t);
            fdate = filterDate(t, now);
            if (fstatus && fpriority && fdate)
            {
                result.append(t);
            }
        }
    }
    return result;
}

bool filter::filterStatus(Task *t)
{
    if (status.count() == 0)
    {
        return true;
    }
    bool result = false;
    foreach (QString s, status)
    {
        if (t->getStatusUUID() == s)
        {
            result = true;
        }
    }
    return result;
}

bool filter::filterPriority(Task *t)
{
    if (priorities.count() == 0)
    {
        return true;
    }
    bool result = false;
    foreach (QString p, priorities)
    {
        if (t->getPriorityUUID() == p)
        {
            result = true;
        }
    }
    return result;
}

bool filter::filterDate(Task *t, QDate now)
{
    switch (expectedForComparator) {
    case 0:
        return true;
    case 1:
        return t->getExpectedFor() < now;
    case 2:
        return t->getExpectedFor() == now;
    case 3:
        return t->getExpectedFor() > now;
    }
    return false;
}

const QJsonObject filter::toJson()
{
    QJsonObject obj;
    obj[name] = name;
    obj[DATE_KEY] = expectedForComparator;
    QJsonArray b;
    foreach (QString uuid, boards)
    {
        b.append(uuid);
    }
    QJsonArray s;
    foreach (QString uuid, status)
    {
        s.append(uuid);
    }
    QJsonArray p;
    foreach (QString uuid, priorities)
    {
        p.append(uuid);
    }
    obj[BOARDS_KEY] = b;
    obj[STATUS_KEY] = s;
    obj[PRIORITY_KEY] = p;
    return obj;
}
