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
#ifdef __linux__
    uint8_t r = rand() % 255;
    uint8_t g = rand() % 255;
    uint8_t b = rand() % 255;
#else
    uint8_t r = arc4random() % 255;
    uint8_t g = arc4random() % 255;
    uint8_t b = arc4random() % 255;
#endif
    return QColor(r, g, b);
}

QColor Tools::getForegroundColor(QColor background)
{
    uint8_t avg = background.red();
    avg += background.green();
    avg += background.blue();
    avg = avg / 3;

    uint8_t avg2 = background.red();
    avg2 += background.green();
    avg2 = avg2 / 2;

    uint8_t avg3 = background.red();
    avg3 += background.blue();
    avg3 = avg3 / 2;

    uint8_t avg4 = background.green();
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
