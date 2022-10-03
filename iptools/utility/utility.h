#ifndef UTILITY_H
#define UTILITY_H

#include "../image/image.h"
#include <sstream>
#include <math.h>

class utility {
public:
    utility();

    virtual ~utility();

    static std::string intToString(int number);

    static int checkValue(int value);

    static float getHue(int red, int green, int blue);

    static float getSaturation(int red, int green, int blue);

    static float getIntensity(int red, int green, int blue);

    static int getRed(float hue, float saturation, float intensity);

    static int getGreen(float hue, float saturation, float intensity);

    static int getBlue(float hue, float saturation, float intensity);

    static void
    histostretch(image &src, image &tgt, int numROI, int *pixelX, int *pixelY, int *sX, int *sY, int *c, int *d);

    static void
    althistostretch(image &src, image &tgt, int numROI, int *pixelX, int *pixelY, int *sX, int *sY, int *c, int *d);

    static void
    histothres(image &src, image &tgt, int numROI, int *pixelX, int *pixelY, int *sX, int *sY, int *t, int *c, int *d);

    static void percchastrech(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3],
                              int sY[3], int *channel, int *c, int *d);

    static void
    rgbstretch(image &src, image &tgt, int numROI, int *pixelX, int *pixelY, int *sX, int *sY, int *c, int *d);

    static void istretch(image &src, image &tgt, int numROI, int *pixelX, int *pixelY, int *sX, int *sY, int *c, int *d);

    static void hstretch(image &src, image &tgt, int numROI, int *pixelX, int *pixelY, int *sX, int *sY, int *c, int *d);

    static void sstretch(image &src, image &tgt, int numROI, int *pixelX, int *pixelY, int *sX, int *sY, int *c, int *d);

    static void
    fullhsistretch(image &src, image &tgt, int numROI, int *pixelX, int *pixelY, int *sX, int *sY, int *c, int *d);
};

#endif