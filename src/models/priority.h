#ifndef PRIORITY_H
#define PRIORITY_H

#include <QString>
#include <QColor>

class Priority
{
public:
    Priority(QString uuid, QString name, QColor color);

    const QString getName();
    const QString getUUID();
    const QColor getColor();

private:
    QString uuid;
    QString name;
    QColor color;

};

#endif // PRIORITY_H
