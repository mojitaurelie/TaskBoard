#ifndef TOOLS_H
#define TOOLS_H

#include <QColor>
#include <QJsonDocument>

class Tools
{
public:
    Tools();

    static const QColor getRandomColor();
    static const QColor getForegroundColor(QColor background);
    static bool isSaveFileExist();
    static bool writeSaveToFile(QJsonDocument doc);
    static bool readSaveFile(QJsonDocument &doc);

private:
    static const QString getSaveFilePath();

};

#endif // TOOLS_H
