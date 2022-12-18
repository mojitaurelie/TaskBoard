#ifndef PRIORITY_H
#define PRIORITY_H

#include <QString>
#include <QColor>
#include <QJsonObject>

class Priority
{
public:
    Priority(QString uuid, QString name, QColor color);
    Priority(QJsonObject);

    const QString getName();
    const QString getUUID();
    const QColor getColor();
    const QJsonObject toJson();

private:
    QString uuid;
    QString name;
    QColor color;

};

#endif // PRIORITY_H
