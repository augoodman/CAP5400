#include "utility.h"

#define MAXRGB 255
#define MINRGB 0
#define MAXLEN 256
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
    int graylevels[256] = {};
    int max;
    image hist1, hist2;
    while (numROI > 0) {
        cout << "Processing ROI #" << ROIcount + 1 << "...\n";
        cout << "ROI location: (" << pixelX[ROIcount] << "," << pixelY[ROIcount] << ")\n";
        cout << "ROI size: " << sX[ROIcount] << "x" << sY[ROIcount] << "\n";
        cout << "ROI function: histostretch" << endl;
        /*create histogram before stretching*/
        for (int i = 0; i < src.getNumberOfRows(); i++)
            for (int j = 0; j < src.getNumberOfColumns(); j++)
                if (i >= pixelY[ROIcount] && i < pixelY[ROIcount] + sY[ROIcount] &&
                    j >= pixelX[ROIcount] && j < pixelX[ROIcount] + sX[ROIcount])
                    graylevels[src.getPixel(i, j)]++;
        max = *max_element(graylevels, graylevels+256);
        cout << "C: " << c[ROIcount] << endl;
        cout << "D: " << d[ROIcount] << "\n\n";
        hist1.resize(256, 256);
        for (int i = 0; i < 256; i++)
            for (int j = 0; j < 256; j++)
                if (i < ((float)graylevels[256 - j]/max) * (float)256)
                    hist1.setPixel(256 - i, 256 - j, 0);
                else hist1.setPixel(256 - i, 256 - j, 255);
        if(ROIcount == 0) hist1.save("before_histostretch_ROI1.pgm");
        else if(ROIcount == 1) hist1.save("before_histostretch_ROI2.pgm");
        else if(ROIcount == 2) hist1.save("before_histostretch_ROI3.pgm");
        for(int i = 0; i < 256; i++) graylevels[i] = 0;
        /*apply stretching to roi*/
        if (ROIcount == 0)
            for (int i = 0; i < src.getNumberOfRows(); i++)
                for (int j = 0; j < src.getNumberOfColumns(); j++)
                    tgt.setPixel(i, j, checkValue(src.getPixel(i, j)));
        for (int i = 0; i < src.getNumberOfRows(); i++)
            for (int j = 0; j < src.getNumberOfColumns(); j++)
                if (i >= pixelY[ROIcount] && i < pixelY[ROIcount] + sY[ROIcount] &&
                    j >= pixelX[ROIcount] && j < pixelX[ROIcount] + sX[ROIcount])
                    tgt.setPixel(i, j, checkValue(
                            (src.getPixel(i, j) - (c[ROIcount]) * 1.05) * ((b - a) / ((d[ROIcount] * 0.95) - (c[ROIcount]) * 1.05))) + a);
        /*create histogram after stretching*/
        for (int i = 0; i < tgt.getNumberOfRows(); i++)
            for (int j = 0; j < tgt.getNumberOfColumns(); j++)
                if (i >= pixelY[ROIcount] && i < pixelY[ROIcount] + sY[ROIcount] &&
                    j >= pixelX[ROIcount] && j < pixelX[ROIcount] + sX[ROIcount])
                    graylevels[tgt.getPixel(i, j)]++;
        max = *max_element(graylevels, graylevels+256);
        hist2.resize(256, 256);
        for (int i = 0; i < 256; i++)
            for (int j = 0; j < 256; j++)
                if (i < ((float)graylevels[256 - j]/max) * (float)256)
                    hist2.setPixel(256 - i, 256 - j, 0);
                else hist2.setPixel(256 - i, 256 - j, 255);
        if(ROIcount == 0) hist2.save("after_histostretch_ROI1.pgm");
        else if (ROIcount == 1) hist2.save("after_histostretch_ROI2.pgm");
        else if (ROIcount == 2) hist2.save("after_histostretch_ROI3.pgm");
        for(int i = 0; i < 256; i++) graylevels[i] = 0;
        --numROI;
        ++ROIcount;
    }
    for(int i = 0; i < 256; i++) graylevels[i] = 0;
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

void utility::histothres(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3], int sY[3], int t[3], int c[3], int d[3]) {
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    int ROIcount = 0;
    int graylevels[256] = {};
    int max;
    image hist1, hist2;
    while (numROI > 0) {
        cout << "Processing ROI #" << ROIcount + 1 << "...\n";
        cout << "ROI location: (" << pixelX[ROIcount] << "," << pixelY[ROIcount] << ")\n";
        cout << "ROI size: " << sX[ROIcount] << "x" << sY[ROIcount] << "\n";
        cout << "ROI function: histothres" << endl;
        /*create histogram before threshold stretching*/
        for (int i = 0; i < src.getNumberOfRows(); i++)
            for (int j = 0; j < src.getNumberOfColumns(); j++)
                if (i >= pixelY[ROIcount] && i < pixelY[ROIcount] + sY[ROIcount] &&
                    j >= pixelX[ROIcount] && j < pixelX[ROIcount] + sX[ROIcount])
                    graylevels[src.getPixel(i, j)]++;
        max = *max_element(graylevels, graylevels+256);
        cout << "C: " << c[ROIcount] << endl;
        cout << "D: " << d[ROIcount] << endl;
        cout << "Threshold: " << t[ROIcount] << "\n\n";
        hist1.resize(256, 256);
        for (int i = 0; i < 256; i++)
            for (int j = 0; j < 256; j++)
                if (i < ((float)graylevels[256 - j]/max) * (float)256)
                    hist1.setPixel(256 - i, 256 - j, 0);
                else hist1.setPixel(256 - i, 256 - j, 255);
        if(ROIcount == 0) hist1.save("before_histothres_ROI1.pgm");
        else if(ROIcount == 1) hist1.save("before_histothres_ROI2.pgm");
        else if(ROIcount == 2) hist1.save("before_histothres_ROI3.pgm");
        for(int i = 0; i < 256; i++) graylevels[i] = 0;
        /*apply stretching to roi*/
        if (ROIcount == 0)
            for (int i = 0; i < src.getNumberOfRows(); i++)
                for (int j = 0; j < src.getNumberOfColumns(); j++)
                    tgt.setPixel(i, j, checkValue(src.getPixel(i, j)));
        for (int i = 0; i < src.getNumberOfRows(); i++)
            for (int j = 0; j < src.getNumberOfColumns(); j++)
                if (i >= pixelY[ROIcount] && i < pixelY[ROIcount] + sY[ROIcount] &&
                    j >= pixelX[ROIcount] && j < pixelX[ROIcount] + sX[ROIcount] && src.getPixel(i,j) < t[ROIcount])
                    tgt.setPixel(i, j, checkValue(
                            (src.getPixel(i, j) - (c[ROIcount]) * 1.05) * ((b - a) / ((d[ROIcount] * 0.95) - (c[ROIcount]) * 1.05))) + a);
        /*create histogram after threshold stretching*/
        for (int i = 0; i < tgt.getNumberOfRows(); i++)
            for (int j = 0; j < tgt.getNumberOfColumns(); j++)
                if (i >= pixelY[ROIcount] && i < pixelY[ROIcount] + sY[ROIcount] &&
                    j >= pixelX[ROIcount] && j < pixelX[ROIcount] + sX[ROIcount])
                    graylevels[tgt.getPixel(i, j)]++;
        max = *max_element(graylevels, graylevels+256);
        hist2.resize(256, 256);
        for (int i = 0; i < 256; i++)
            for (int j = 0; j < 256; j++)
                if (i < ((float)graylevels[256 - j]/max) * (float)256)
                    hist2.setPixel(256 - i, 256 - j, 0);
                else hist2.setPixel(256 - i, 256 - j, 255);
        if(ROIcount == 0) hist2.save("after_histothres_ROI1.pgm");
        else if (ROIcount == 1) hist2.save("after_histothres_ROI2.pgm");
        else if (ROIcount == 2) hist2.save("after_histothres_ROI3.pgm");
        for(int i = 0; i < 256; i++) graylevels[i] = 0;
        --numROI;
        ++ROIcount;
    }
    for(int i = 0; i < 256; i++) graylevels[i] = 0;
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