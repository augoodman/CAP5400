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
void utility::addGrey(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3], int sY[3], int value[3])
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
                       bool isDouble[3], int threshold1[3], int threshold2[3])
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    int ROIcount = 1;
    while (numROI > 0)
    {
        for (int i = 0; i < src.getNumberOfRows(); i++)
            for (int j = 0; j < src.getNumberOfColumns(); j++)
                if (!isDouble[ROIcount - 1]) {
                    if (i >= pixelY[ROIcount - 1] && i < pixelY[ROIcount - 1] + sY[ROIcount - 1] &&
                    j >= pixelX[ROIcount - 1] && j < pixelX[ROIcount - 1] + sX[ROIcount - 1] &&
                    src.getPixel(i, j) < threshold1[ROIcount - 1])
                        tgt.setPixel(i, j, MINRGB);
                    else if (i >= pixelY[ROIcount - 1] && i < pixelY[ROIcount - 1] + sY[ROIcount - 1] &&
                    j >= pixelX[ROIcount - 1] && j < pixelX[ROIcount - 1] + sX[ROIcount - 1] &&
                    src.getPixel(i, j) >= threshold1[ROIcount - 1])
                        tgt.setPixel(i, j, MAXRGB);
                    else if (ROIcount == 1)
                        tgt.setPixel(i, j, checkValue(src.getPixel(i, j)));
                }
                else
                {
                    if (i >= pixelY[ROIcount - 1] && i < pixelY[ROIcount - 1] + sY[ROIcount - 1] &&
                    j >= pixelX[ROIcount - 1] && j < pixelX[ROIcount - 1] + sX[ROIcount - 1] &&
                    src.getPixel(i, j) < threshold1[ROIcount - 1])
                        tgt.setPixel(i, j, MINRGB);
                    else if (i >= pixelY[ROIcount - 1] && i < pixelY[ROIcount - 1] + sY[ROIcount - 1] &&
                    j >= pixelX[ROIcount - 1] && j < pixelX[ROIcount - 1] + sX[ROIcount - 1] &&
                    src.getPixel(i, j) > threshold2[ROIcount - 1])
                        tgt.setPixel(i, j, MINRGB);
                    else if (i >= pixelY[ROIcount - 1] && i < pixelY[ROIcount - 1] + sY[ROIcount - 1] &&
                    j >= pixelX[ROIcount - 1] && j < pixelX[ROIcount - 1] + sX[ROIcount - 1] &&
                    src.getPixel(i, j) > threshold1[ROIcount - 1] && src.getPixel(i, j ) < threshold2[ROIcount - 1])
                        tgt.setPixel(i, j, MAXRGB);
                    else if (ROIcount == 1)
                        tgt.setPixel(i, j, checkValue(src.getPixel(i, j)));
                }
        --numROI;
        ++ROIcount;
    }
}

/*-----------------------------------------------------------------------
void utility::doubleThreshold(image &src, image &tgt, int threshold)
{
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    for (int i=0; i<src.getNumberOfRows(); i++)
    {
        for (int j=0; j<src.getNumberOfColumns(); j++)
        {
            if (src.getPixel(i,j) > threshold + 50)
                tgt.setPixel(i,j,MINRGB);
            else if (src.getPixel(i,j) < threshold - 50)
                tgt.setPixel(i,j,MINRGB);
            else
                tgt.setPixel(i,j,MAXRGB);
        }
    }
}
-----------------------------------------------------------------------**/

/*-----------------------------------------------------------------------**/
void utility::scale(image &src, image &tgt, float ratio)
{
	int rows = (int)((float)src.getNumberOfRows() * ratio);
	int cols  = (int)((float)src.getNumberOfColumns() * ratio);
	tgt.resize(rows, cols);
	for (int i=0; i<rows; i++)
	{
		for (int j=0; j<cols; j++)
		{	
			/* Map the pixel of new image back to original image */
			int i2 = (int)floor((float)i/ratio);
			int j2 = (int)floor((float)j/ratio);
			if (ratio == 2) {
				/* Directly copy the value */
				tgt.setPixel(i,j,checkValue(src.getPixel(i2,j2)));
			}

			if (ratio == 0.5) {
				/* Average the values of four pixels */
				int value = src.getPixel(i2,j2) + src.getPixel(i2,j2+1) + src.getPixel(i2+1,j2) + src.getPixel(i2+1,j2+1);
				tgt.setPixel(i,j,checkValue(value/4));
			}
		}
	}
}

void utility::uniformSmoothing(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3], int sY[3], int ws[3])
{
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    int ROIcount = 1;
    while (numROI > 0)
    {
        if (ws[ROIcount - 1] % 2 == 0)
            ws[ROIcount -1] += 1;
        for (int i = 0; i < src.getNumberOfRows(); i++)
            for (int j = 0; j < src.getNumberOfColumns(); j++)
                if (i >= pixelY[ROIcount - 1] && i < pixelY[ROIcount - 1] + sY[ROIcount - 1] &&
                    j >= pixelX[ROIcount - 1] && j < pixelX[ROIcount - 1] + sX[ROIcount - 1])
                {
                    //for loop to iterate through window summing gray values
                    int sum = 0;
                    for (int k = 0; k < ws[ROIcount - 1]; k++)
                        for (int l = 0; l < ws[ROIcount - 1]; l++)
                            if (i + k <= src.getNumberOfColumns() + 1 && j + l <= src.getNumberOfRows() + 1)
                                sum += src.getPixel(i + k, j + l);
                    //divide sum by ws^2
                    tgt.setPixel(i, j, checkValue(sum / (ws[ROIcount - 1] * ws[ROIcount - 1])));
                }
                else if (ROIcount == 1)
                    tgt.setPixel(i, j, checkValue(src.getPixel(i, j)));
        --numROI;
        ++ROIcount;
    }
}

void utility::adaptiveSmoothing(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3], int sY[3], int ws[3], int threshold[3])
{

}

void utility::moreColor(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3], int sY[3], int moreC[3])
{

}