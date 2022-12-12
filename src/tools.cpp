#include "tools.h"

#define BRIGHTNESS_THRESHOLD 170
#define WHITECOLOR 240
#define BLACKCOLOR 15

Tools::Tools()
{

}

QColor Tools::getRandomColor()
{
    srand(time(0));
    int r = arc4random() % 255;
    int g = arc4random() % 255;
    int b = arc4random() % 255;
    return QColor(r, g, b);
}

QColor Tools::getForegroundColor(QColor background)
{
    int avg = background.red();
    avg += background.green();
    avg += background.blue();
    avg = avg / 3;

    int avg2 = background.red();
    avg2 += background.green();
    avg2 = avg2 / 2;

    int avg3 = background.red();
    avg3 += background.blue();
    avg3 = avg3 / 2;

    int avg4 = background.green();
    avg4 += background.blue();
    avg4 = avg4 / 2;

    if (avg < BRIGHTNESS_THRESHOLD && avg2 < BRIGHTNESS_THRESHOLD && avg3 < BRIGHTNESS_THRESHOLD && avg4 < BRIGHTNESS_THRESHOLD)
    {
        return QColor(WHITECOLOR, WHITECOLOR, WHITECOLOR);
    }
    else
    {
        return QColor(BLACKCOLOR, BLACKCOLOR, BLACKCOLOR);
    }
}
