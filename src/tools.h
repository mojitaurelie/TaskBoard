#ifndef TOOLS_H
#define TOOLS_H

#include <QColor>

class Tools
{
public:
    Tools();

    static QColor getRandomColor();
    static QColor getForegroundColor(QColor background);

};

#endif // TOOLS_H
