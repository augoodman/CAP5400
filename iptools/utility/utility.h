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
		static void binarize(image &src, image &tgt, int threshold);
        static void doubleThreshold(image &src, image &tgt, int threshold);
		static void scale(image &src, image &tgt, float ratio);
};

#endif

