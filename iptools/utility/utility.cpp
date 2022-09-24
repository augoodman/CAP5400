#include "utility.h"

#define MAXRGB 255
#define MINRGB 0

std::string utility::intToString(int number) {
    std::stringstream ss;//create a stringstream
    ss << number;//add number to the stream
    return ss.str();//return a string with the contents of the stream
}

int utility::checkValue(int value) {
    if (value > MAXRGB)
        return MAXRGB;
    if (value < MINRGB)
        return MINRGB;
    return value;
}

bool utility::isInBounds(int x, int y, image &src) {
    if (x <= src.getNumberOfRows() && y <= src.getNumberOfColumns())
        return true;
    return false;
}

/*-----------------------------------------------------------------------**/
void utility::histostretch(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3], int sY[3], int a[3], int b[3]) {
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    int ROIcount = 1;
    int graylevels[256] = {};
    int max, value, sum = 0;
    image hist1, hist2;
    while (numROI > 0) {
        for (int i = 0; i < src.getNumberOfRows(); i++)
            for (int j = 0; j < src.getNumberOfColumns(); j++)
                if (i >= pixelY[ROIcount - 1] && i < pixelY[ROIcount - 1] + sY[ROIcount - 1] &&
                    j >= pixelX[ROIcount - 1] && j < pixelX[ROIcount - 1] + sX[ROIcount - 1]) {
                    graylevels[src.getPixel(pixelX[ROIcount - 1] + i, pixelY[ROIcount - 1] + j)]++;
                    //tgt.setPixel(i, j, checkValue(src.getPixel(i, j) + a[ROIcount - 1]));
                }
                else if (ROIcount == 1)
                    tgt.setPixel(i, j, checkValue(src.getPixel(i, j)));
        max = *max_element(graylevels, graylevels+256);
        for (int i = 0; i < 256; i++)
            if(graylevels[i] == max) value = i;
        char* histbefore, histafter;
        if(ROIcount == 1) histbefore = "histostretch_ROI1_before.pgm";
        else if (ROIcount == 2) histbefore = "histostretch_ROI2_before.pgm";
        else if (ROIcount == 3) histbefore = "histostretch_ROI3_before.pgm";
        hist1.resize(256, 256);
        for (int i = 0; i < 256; i++)
            for (int j = 0; j < 256; j++)
                if (i < ((float)graylevels[j]/max) * (float)256)
                    hist1.setPixel(256 - i, 256 - j, 0);
                else hist1.setPixel(256 - i, 256 - j, 255);
        hist1.save(histbefore);
        --numROI;
        ++ROIcount;
    }
}

/*-----------------------------------------------------------------------**/
void utility::althistostretch(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3], int sY[3],
                       int threshold[3]) {
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    int ROIcount = 1;
    while (numROI > 0) {
        for (int i = 0; i < src.getNumberOfRows(); i++)
            for (int j = 0; j < src.getNumberOfColumns(); j++) {
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

void utility::histothres(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3], int sY[3],
                            int ws[3]) {
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    int ROIcount = 1;
    int outside = 0;
    while (numROI > 0) {
        if (ws[ROIcount - 1] % 2 == 0)
            ws[ROIcount - 1] += 1;
        for (int i = 0; i < src.getNumberOfRows(); i++)
            for (int j = 0; j < src.getNumberOfColumns(); j++)
                if (i >= pixelY[ROIcount - 1] && i < pixelY[ROIcount - 1] + sY[ROIcount - 1] &&
                    j >= pixelX[ROIcount - 1] && j < pixelX[ROIcount - 1] + sX[ROIcount - 1]) {
                    int sum = 0;
                    for (int k = 0; k < ws[ROIcount - 1]; k++)
                        for (int l = 0; l < ws[ROIcount - 1]; l++)
                            if (i + k <= src.getNumberOfColumns() + 1 && j + l <= src.getNumberOfRows() + 1)
                                sum += src.getPixel(i + k - ws[ROIcount - 1] / 2, j + l - ws[ROIcount - 1] / 2);
                            else
                                ++outside;
                    tgt.setPixel(i, j, checkValue(sum / ((ws[ROIcount - 1] * ws[ROIcount - 1]) - outside)));
                    outside = 0;
                } else if (ROIcount == 1)
                    tgt.setPixel(i, j, checkValue(src.getPixel(i, j)));
        --numROI;
        ++ROIcount;
    }
}

void utility::percchastretch(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3], int sY[3], int ws[3],
                     int threshold[3]) {
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    int ROIcount = 1;
    int outside = 0;
    while (numROI > 0) {
        if (ws[ROIcount - 1] % 2 == 0)
            ws[ROIcount - 1] += 1;
        for (int i = 0; i < src.getNumberOfRows(); i++)
            for (int j = 0; j < src.getNumberOfColumns(); j++)
                if (i >= pixelY[ROIcount - 1] && i < pixelY[ROIcount - 1] + sY[ROIcount - 1] &&
                    j >= pixelX[ROIcount - 1] && j < pixelX[ROIcount - 1] + sX[ROIcount - 1]) {
                    int sum = 0;
                    for (int k = 0; k < ws[ROIcount - 1]; k++)
                        for (int l = 0; l < ws[ROIcount - 1]; l++) {
                            if (i + k <= src.getNumberOfColumns() + 1 && j + l <= src.getNumberOfRows() + 1)
                                sum += src.getPixel(i + k - ws[ROIcount - 1] / 2, j + l - ws[ROIcount - 1] / 2);
                            else
                                ++outside;
                            if (abs((sum / (ws[ROIcount - 1] * ws[ROIcount - 1])) - src.getPixel(i + k, j + l)) <
                                threshold[ROIcount - 1])
                                tgt.setPixel(i, j, checkValue(sum / ((ws[ROIcount - 1] * ws[ROIcount - 1]) - outside)));
                            else
                                tgt.setPixel(i, j, checkValue(src.getPixel(i, j)));
                            outside = 0;
                        }
                } else if (ROIcount == 1)
                    tgt.setPixel(i, j, checkValue(src.getPixel(i, j)));
        --numROI;
        ++ROIcount;
    }
}

void utility::rgbstretch(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3], int sY[3],
                             float redScale[3], float greenScale[3], float blueScale[3]) {
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    int ROIcount = 1;
    if (redScale[ROIcount - 1] < 0)
        redScale[ROIcount - 1] *= -1;
    if (greenScale[ROIcount - 1] < 0)
        greenScale[ROIcount - 1] *= -1;
    if (blueScale[ROIcount - 1] < 0)
        blueScale[ROIcount - 1] *= -1;
    while (numROI > 0) {
        for (int i = 0; i < src.getNumberOfRows(); i++)
            for (int j = 0; j < src.getNumberOfColumns(); j++)
                if (i >= pixelY[ROIcount - 1] && i < pixelY[ROIcount - 1] + sY[ROIcount - 1] &&
                    j >= pixelX[ROIcount - 1] && j < pixelX[ROIcount - 1] + sX[ROIcount - 1]) {
                    tgt.setPixel(i, j, RED, checkValue(src.getPixel(i, j, RED) * abs(redScale[ROIcount - 1])));
                    tgt.setPixel(i, j, GREEN, checkValue(src.getPixel(i, j, GREEN) * abs(greenScale[ROIcount - 1])));
                    tgt.setPixel(i, j, BLUE, checkValue(src.getPixel(i, j, BLUE) * abs(blueScale[ROIcount - 1])));
                } else if (ROIcount == 1) {
                    tgt.setPixel(i, j, RED, checkValue(src.getPixel(i, j, RED)));
                    tgt.setPixel(i, j, GREEN, checkValue(src.getPixel(i, j, GREEN)));
                    tgt.setPixel(i, j, BLUE, checkValue(src.getPixel(i, j, BLUE)));
                }
        --numROI;
        ++ROIcount;
    }
}

/*-----------------------------------------------------------------------**/
void utility::istretch(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3], int sY[3], int value[3]) {
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    int ROIcount = 1;
    while (numROI > 0) {
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
void utility::hstretch(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3], int sY[3], int value[3]) {
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    int ROIcount = 1;
    while (numROI > 0) {
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
void utility::sstretch(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3], int sY[3], int value[3]) {
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    int ROIcount = 1;
    while (numROI > 0) {
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
void utility::fullhsistretch(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3], int sY[3], int value[3]) {
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    int ROIcount = 1;
    while (numROI > 0) {
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