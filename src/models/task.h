#ifndef TASK_H
#define TASK_H

#include <QString>
#include <QDate>
#include <QJsonObject>

class Task
{
public:
    Task(QString title, QString description, QDate expectedFor, QString priorityUUID, QString statusUUID);
    Task(QJsonObject);

    const QString getTitle();
    const QString getDescription();
    const QDate getExpectedFor();
    const QString getPriorityUUID();
    const QString getStatusUUID();
    const QJsonObject toJson();

    void update(Task);

private:
    QString title;
    QString description;
    QDate expectedFor;
    QString priorityUUID;
    QString statusUUID;
};

#endif // TASK_H
