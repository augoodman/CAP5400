#ifndef UTILITY_H
#define UTILITY_H

#include "../image/image.h"
#include <sstream>
#include <math.h>

class utility
{
	public:
		utility();
		virtual ~utility();
		static std::string intToString(int number);
		static int checkValue(int value);
        static bool isInBounds(int x, int y, image &src);
        static void addGrey(image &src, image &tgt, int numROI, int *pixelX, int *pixelY, int *sX, int *sY, int *value);
		static void binarize(image &src, image &tgt, int numROI, int *pixelX, int *pixelY, int *sX, int *sY,
            bool *isDouble, int *threshold1, int *threshold2);
        //static void doubleThreshold(image &src, image &tgt, int threshold);
		static void scale(image &src, image &tgt, float ratio);
        static void uniformSmoothing(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3],
            int sY[3], int ws[3]);
        static void adaptiveSmoothing(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3],
            int sY[3], int ws[3], int threshold[3]);
        static void moreColor(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3], int sY[3],
            float moreC[3]);
};

#endif

