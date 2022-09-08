#include "utility.h"

#define MAXRGB 255
#define MINRGB 0

std::string utility::intToString(int number)
{
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

int utility::checkValue(int value)
{
	if (value > MAXRGB)
		return MAXRGB;
	if (value < MINRGB)
		return MINRGB;
	return value;
}

bool utility::isInBounds(int x, int y, image &src)
{
    if (x <= src.getNumberOfRows() && y <= src.getNumberOfColumns())
        return true;
    return false;
}

/*-----------------------------------------------------------------------**/
void utility::add(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3], int sY[3], int value[3])
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    int ROIcount = 1;
    while (numROI > 0)
    {
        for (int i = 0; i < src.getNumberOfRows(); i++)
            for (int j = 0; j < src.getNumberOfColumns(); j++)
                if (i >= pixelY[ROIcount - 1] && i < pixelY[ROIcount - 1] + sY[ROIcount - 1] &&
                    j >= pixelX[ROIcount - 1] && j < pixelX[ROIcount - 1] + sX[ROIcount - 1])
                    tgt.setPixel(i, j, checkValue(src.getPixel(i, j) + value[ROIcount - 1]));
                else if (ROIcount == 1)
                    tgt.setPixel(i, j, checkValue(src.getPixel(i, j)));
        --numROI;
        ++ROIcount;
    }
}

/*-----------------------------------------------------------------------**/
void utility::binarize(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3], int sY[3],
                       int threshold[3])
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    int ROIcount = 1;
    while (numROI > 0)
    {
        for (int i = 0; i < src.getNumberOfRows(); i++)
            for (int j = 0; j < src.getNumberOfColumns(); j++)
            {
                if (i >= pixelY[ROIcount - 1] && i < pixelY[ROIcount - 1] + sY[ROIcount - 1] &&
                j >= pixelX[ROIcount - 1] && j < pixelX[ROIcount - 1] + sX[ROIcount - 1] &&
                src.getPixel(i, j) < threshold[ROIcount - 1])
                    tgt.setPixel(i, j, MINRGB);
                else if (i >= pixelY[ROIcount - 1] && i < pixelY[ROIcount - 1] + sY[ROIcount - 1] &&
                j >= pixelX[ROIcount - 1] && j < pixelX[ROIcount - 1] + sX[ROIcount - 1] &&
                src.getPixel(i, j) >= threshold[ROIcount - 1])
                    tgt.setPixel(i, j, MAXRGB);
                else if (ROIcount == 1)
                    tgt.setPixel(i, j, checkValue(src.getPixel(i, j)));
            }
        --numROI;
        ++ROIcount;
    }
}

void utility::uniformsmooth(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3], int sY[3], int ws[3])
{
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    int ROIcount = 1;
    int outside = 0;
    while (numROI > 0)
    {
        if (ws[ROIcount - 1] % 2 == 0)
            ws[ROIcount -1] += 1;
        for (int i = 0; i < src.getNumberOfRows(); i++)
            for (int j = 0; j < src.getNumberOfColumns(); j++)
                if (i >= pixelY[ROIcount - 1] && i < pixelY[ROIcount - 1] + sY[ROIcount - 1] &&
                    j >= pixelX[ROIcount - 1] && j < pixelX[ROIcount - 1] + sX[ROIcount - 1])
                {
                    int sum = 0;
                    for (int k = 0; k < ws[ROIcount - 1]; k++)
                        for (int l = 0; l < ws[ROIcount - 1]; l++)
                            if (i + k <= src.getNumberOfColumns() && j + l <= src.getNumberOfRows())
                                sum += src.getPixel(i + k, j + l);
                            else
                                ++outside;
                    tgt.setPixel(i, j, checkValue(sum / ((ws[ROIcount - 1] * ws[ROIcount - 1]) - outside)));
                    outside = 0;
                }
                else if (ROIcount == 1)
                    tgt.setPixel(i, j, checkValue(src.getPixel(i, j)));
        --numROI;
        ++ROIcount;
    }
}

void utility::adptvsmooth(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3], int sY[3], int ws[3], int threshold[3])
{
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    int ROIcount = 1;
    int outside = 0;
    while (numROI > 0)
    {
        if (ws[ROIcount - 1] % 2 == 0)
            ws[ROIcount -1] += 1;
        for (int i = 0; i < src.getNumberOfRows(); i++)
            for (int j = 0; j < src.getNumberOfColumns(); j++)
                if (i >= pixelY[ROIcount - 1] && i < pixelY[ROIcount - 1] + sY[ROIcount - 1] &&
                    j >= pixelX[ROIcount - 1] && j < pixelX[ROIcount - 1] + sX[ROIcount - 1])
                {
                    int sum = 0;
                    for (int k = 0; k < ws[ROIcount - 1]; k++)
                        for (int l = 0; l < ws[ROIcount - 1]; l++)
                        {
                            if (i + k <= src.getNumberOfColumns() + 1 && j + l <= src.getNumberOfRows() + 1)
                                sum += src.getPixel(i + k, j + l);
                            else
                                ++outside;
                            if (abs((sum / (ws[ROIcount - 1] * ws[ROIcount - 1])) - src.getPixel(i+k, j+l)) <
                                threshold[ROIcount - 1])
                                tgt.setPixel(i, j, checkValue(sum / ((ws[ROIcount - 1] * ws[ROIcount - 1]) - outside)));
                            else
                                tgt.setPixel(i, j, checkValue(src.getPixel(i, j)));
                            outside = 0;
                        }
                }
                else if (ROIcount == 1)
                    tgt.setPixel(i, j, checkValue(src.getPixel(i, j)));
        --numROI;
        ++ROIcount;
    }
}

void utility::mulcolorbright(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3], int sY[3], float redScale[3], float greenScale[3], float blueScale[3])
{
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    int ROIcount = 1;
    while (numROI > 0)
    {
        for (int i = 0; i < src.getNumberOfRows(); i++)
            for (int j = 0; j < src.getNumberOfColumns(); j++)
                if (i >= pixelY[ROIcount - 1] && i < pixelY[ROIcount - 1] + sY[ROIcount - 1] &&
                    j >= pixelX[ROIcount - 1] && j < pixelX[ROIcount - 1] + sX[ROIcount - 1])
                {
                    tgt.setPixel(i, j, RED, checkValue(src.getPixel(i, j, RED) * redScale[ROIcount - 1]));
                    tgt.setPixel(i, j, GREEN, checkValue(src.getPixel(i, j, GREEN) * greenScale[ROIcount - 1]));
                    tgt.setPixel(i, j, BLUE, checkValue(src.getPixel(i, j, BLUE) * blueScale[ROIcount - 1]));
                }
                else if (ROIcount == 1)
                {
                    tgt.setPixel(i, j, RED, checkValue(src.getPixel(i, j, RED)));
                    tgt.setPixel(i, j, GREEN, checkValue(src.getPixel(i, j, GREEN)));
                    tgt.setPixel(i, j, BLUE, checkValue(src.getPixel(i, j, BLUE)));
                }
        --numROI;
        ++ROIcount;
    }
}