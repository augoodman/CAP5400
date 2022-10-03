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

    static bool isInBounds(int x, int y, image &src);

    static void add(image &src, image &tgt, int numROI, int *pixelX, int *pixelY, int *sX, int *sY, int *value);

    static void binarize(image &src, image &tgt, int numROI, int *pixelX, int *pixelY, int *sX, int *sY,
                         int *threshold);

    static void uniformsmooth(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3],
                              int sY[3], int ws[3]);

    static void adptvsmooth(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3],
                            int sY[3], int ws[3], int threshold[3]);

    static void mulcolorbright(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3], int sY[3],
                               float redScale[3], float greenScale[3], float blueScale[3]);
};

#endif

