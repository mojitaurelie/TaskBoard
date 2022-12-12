#ifndef TASK_H
#define TASK_H

#include <QString>
#include <QDate>

class Task
{
public:
    Task(QString title, QString description, QDate expectedFor, QString priorityUUID, QString statusUUID);

    const QString getTitle();
    const QString getDescription();
    const QDate getExpectedFor();
    const QString getPriorityUUID();
    const QString getStatusUUID();

    void update(Task);

private:
    QString title;
    QString description;
    QDate expectedFor;
    QString priorityUUID;
    QString statusUUID;
};

#endif // TASK_H