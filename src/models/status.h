#ifndef STATUS_H
#define STATUS_H

#include <QString>
#include <QColor>
#include <QJsonObject>

class Status
{
public:
    Status(QString uuid, QString name, QColor color);
    Status(QJsonObject);

    const QString getName();
    const QString getUUID();
    const QColor getColor();
    const QJsonObject toJson();

private:
    QString uuid;
    QString name;
    QColor color;
};

#endif // STATUS_H
