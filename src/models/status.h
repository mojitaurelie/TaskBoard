#ifndef STATUS_H
#define STATUS_H

#include <QString>
#include <QColor>

class Status
{
public:
    Status(QString uuid, QString name, QColor color);

    const QString getName();
    const QString getUUID();
    const QColor getColor();

private:
    QString uuid;
    QString name;
    QColor color;
};

#endif // STATUS_H
