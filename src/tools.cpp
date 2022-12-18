#include "tools.h"

#define BRIGHTNESS_THRESHOLD 170
#define WHITECOLOR 240
#define BLACKCOLOR 15
#define FILENAME "data.json"

#include <QStandardPaths>
#include <QDir>

Tools::Tools()
{

}

const QColor Tools::getRandomColor()
{
    srand(time(0));
#ifdef __APPLE__
    uint8_t r = arc4random() % 255;
    uint8_t g = arc4random() % 255;
    uint8_t b = arc4random() % 255;
#else
    uint8_t r = rand() % 255;
    uint8_t g = rand() % 255;
    uint8_t b = rand() % 255;
#endif
    return QColor(r, g, b);
}

const QColor Tools::getForegroundColor(QColor background)
{
    uint8_t avg = (background.red() + background.green() + background.blue()) / 3;
    uint8_t avg2 = (background.red() + background.green()) / 2;
    uint8_t avg3 = (background.red() + background.blue()) / 2;
    uint8_t avg4 = (background.green() + background.blue()) / 2;

    if (avg < BRIGHTNESS_THRESHOLD && avg2 < BRIGHTNESS_THRESHOLD && avg3 < BRIGHTNESS_THRESHOLD && avg4 < BRIGHTNESS_THRESHOLD)
    {
        return QColor(WHITECOLOR, WHITECOLOR, WHITECOLOR);
    }
    else
    {
        return QColor(BLACKCOLOR, BLACKCOLOR, BLACKCOLOR);
    }
}

const QString Tools::getSaveFilePath() {
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (!QDir(path).exists())
    {
        QDir().mkpath(path);
    }

    path += "/data/";
    if (!QDir(path).exists())
    {
        QDir().mkpath(path);
    }
    path += FILENAME;
    return QDir::cleanPath(path);
}

bool Tools::isSaveFileExist()
{
    return QFile::exists(Tools::getSaveFilePath());
}

bool Tools::writeSaveToFile(QJsonDocument doc)
{
    bool success = false;
    QFile *f = new QFile(getSaveFilePath());
    if (f->open(QIODevice::WriteOnly))
    {
        f->write(doc.toJson());
        f->close();
        success = true;
    }
    delete f;
    return success;
}

bool Tools::readSaveFile(QJsonDocument &doc) {
    QFile* file = new QFile(getSaveFilePath());
    if (!file->open(QIODevice::ReadOnly))
    {
        file->close();
        delete file;
        return false;
    }
    QString json = QString(file->readAll());
    file->close();
    delete file;

    doc = QJsonDocument::fromJson(json.toUtf8());
    return true;
}
