#include "utility.h"

#define MAXRGB 255
#define MINRGB 0
#define a 0
#define b 255

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
void utility::histostretch(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3], int sY[3], int c[3], int d[3]) {
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    int ROIcount = 0;
    int srcgraylevels[256] = {};
    int tgtgraylevels[256] = {};
    int max;
    image hist1, hist2;
    while (numROI > 0) {
        /*create histogram before stretching*/
        for (int i = 0; i < src.getNumberOfRows(); i++)
            for (int j = 0; j < src.getNumberOfColumns(); j++)
                if (i >= pixelY[ROIcount] && i < pixelY[ROIcount] + sY[ROIcount] &&
                    j >= pixelX[ROIcount] && j < pixelX[ROIcount] + sX[ROIcount])
                    srcgraylevels[src.getPixel(i, j)]++;
        max = *max_element(srcgraylevels, srcgraylevels+256);
        cout << "Number of rows: " << src.getNumberOfRows() << endl;
        cout << "Number of columns: " << src.getNumberOfColumns() << endl;
        cout << "A: " << a << endl;
        cout << "B: " << b << endl;
        cout << "Maxima before: " << max << endl;
        char* histbefore;
        if(ROIcount == 0) histbefore = "histostretch_ROI1_before.pgm";
        else if (ROIcount == 1) histbefore = "histostretch_ROI2_before.pgm";
        else if (ROIcount == 2) histbefore = "histostretch_ROI3_before.pgm";
        hist1.resize(256, 256);
        for (int i = 0; i < 256; i++)
            for (int j = 0; j < 256; j++)
                if (i < ((float)srcgraylevels[256 - j]/max) * (float)256)
                    hist1.setPixel(256 - i, 256 - j, 0);
                else hist1.setPixel(256 - i, 256 - j, 255);
        hist1.save(histbefore);
        /*apply stretching to roi*/
        for (int i = 0; i < src.getNumberOfRows(); i++)
            for (int j = 0; j < src.getNumberOfColumns(); j++)
                if (i >= pixelY[ROIcount] && i < pixelY[ROIcount] + sY[ROIcount] &&
                    j >= pixelX[ROIcount] && j < pixelX[ROIcount] + sX[ROIcount])
                    tgt.setPixel(i, j, checkValue(
                            (src.getPixel(i, j) - (c[ROIcount]) * 1.05) * ((b - a) / ((d[ROIcount] * 0.95) - (c[ROIcount]) * 1.05))) + a);
                else if (ROIcount == 1)
                    tgt.setPixel(i, j, checkValue(src.getPixel(i, j)));
        /*create histogram after stretching*/
        for (int i = 0; i < tgt.getNumberOfRows(); i++)
            for (int j = 0; j < tgt.getNumberOfColumns(); j++)
                if (i >= pixelY[ROIcount] && i < pixelY[ROIcount] + sY[ROIcount] &&
                    j >= pixelX[ROIcount] && j < pixelX[ROIcount] + sX[ROIcount])
                    tgtgraylevels[tgt.getPixel(i, j)]++;
        max = *max_element(tgtgraylevels, tgtgraylevels+256);
        cout << "Maxima after: " << max << endl;
        char* histafter;
        if(ROIcount == 0) histafter = "histostretch_ROI1_after.pgm";
        else if (ROIcount == 1) histafter = "histostretch_ROI2_after.pgm";
        else if (ROIcount == 2) histafter = "histostretch_ROI3_after.pgm";
        hist2.resize(256, 256);
        for (int i = 0; i < 256; i++)
            for (int j = 0; j < 256; j++)
                if (i < ((float)tgtgraylevels[256 - j]/max) * (float)256)
                    hist2.setPixel(256 - i, 256 - j, 0);
                else hist2.setPixel(256 - i, 256 - j, 255);
        hist2.save(histafter);
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