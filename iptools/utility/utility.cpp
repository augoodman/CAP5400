#include "utility.h"
#include <math.h>

#define MAXRGB 255
#define MINRGB 0
#define a 0
#define b 255

const float PI = atan(1) * 4;

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

float utility::getHue(int red, int green, int blue) {
    float hue;
    float nr = (float) red / (float) (red + green + blue);
    float ng = (float) green / (float) (red + green + blue);
    float nb = (float) blue / (float) (red + green + blue);
    hue = acos((0.5 * ((nr - ng) + (nr - nb))) / (sqrt(((nr - ng)) * (nr - ng) + ((nr - nb) * (ng - nb)))));
    if (blue > green)
        hue = (2 * PI) - hue;
    return hue * 180 / PI;
}

float utility::getSaturation(int red, int green, int blue) {
    float saturation;
    float nr = (float) red / (float) (red + green + blue);
    float ng = (float) green / (float) (red + green + blue);
    float nb = (float) blue / (float) (red + green + blue);
    saturation = 1 - (3 * min(nr, min(ng, nb)));
    return saturation * 100;
}

float utility::getIntensity(int red, int green, int blue) {
    float intensity = (float) (red + green + blue) / (float) (3 * b);
    return intensity * b;
}

int utility::getRed(float hue, float saturation, float intensity) {
    float red;
    float nh = hue * (PI / 180);
    float ns = saturation / 100;
    float ni = intensity / b;
    if (nh < 2 * PI / 3)
        red = ni * (1 + (ns * cos(nh)) / cos(PI / 3 - nh));
    else if (nh < 4 * PI / 3) {
        nh = nh - 2 * PI / 3;
        red = ni * (1 - ns);
    }
    else {
        nh = nh - 4 * PI / 3;
        red = 3 * ni - ((ni * (1 - ns)) + (ni * (1 + (ns * cos(nh)) / cos(PI / 3 - nh))));
    }
    return red * 255;
}

int utility::getGreen(float hue, float saturation, float intensity) {
    float green;
    float nh = hue * (PI / 180);
    float ns = saturation / 100;
    float ni = intensity / b;
    if (nh < 2 * PI / 3)
        green = 3 * ni - ((ni * (1 - ns)) + (ni * (1 + (ns * cos(nh)) / cos(PI / 3 - nh))));
    else if (nh < 4 * PI / 3) {
        nh = nh - 2 * PI / 3;
        green = ni * (1 + (ns * cos(nh)) / cos(PI / 3 - nh));
    }
    else {
        nh = nh - 4 * PI / 3;
        green = ni * (1 - ns);
    }
    return green * 255;
}

int utility::getBlue(float hue, float saturation, float intensity) {
    float blue;
    float nh = hue * (PI / 180);
    float ns = saturation / 100;
    float ni = intensity / b;
    if (nh < 2 * PI / 3)
        blue = ni * (1 - ns);
    else if (nh < 4 * PI / 3) {
        nh = nh - 2 * PI / 3;
        blue = 3 * ni - ((ni * (1 - ns)) + (ni * (1 + (ns * cos(nh)) / cos(PI / 3 - nh))));
    }
    else {
        nh = nh - 4 * PI / 3;
        blue = ni * (1 + (ns * cos(nh)) / cos(PI / 3 - nh));
    }
    return blue * 255;
}

/*-----------------------------------------------------------------------**/
void
utility::histostretch(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3], int sY[3], int c[3],
                      int d[3]) {
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    int ROIcount = 0;
    while (numROI > 0) {
        cout << "Processing ROI #" << ROIcount + 1 << "...\n";
        cout << "ROI location: (" << pixelX[ROIcount] << "," << pixelY[ROIcount] << ")\n";
        cout << "ROI size: " << sX[ROIcount] << "x" << sY[ROIcount] << "\n";
        cout << "ROI function: histostretch" << endl;
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
                            (src.getPixel(i, j) - (c[ROIcount]) * 1.05) *
                            ((b - a) / ((d[ROIcount] * 0.95) - (c[ROIcount]) * 1.05))) + a);

        --numROI;
        ++ROIcount;
    }
    cout << endl;
}

/*-----------------------------------------------------------------------**/
void utility::althistostretch(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3], int sY[3],
                              int c[3], int d[3]) {
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    int ROIcount = 0;
    int graylevels[256] = {};
    int max;
    image hist1, hist2;
    while (numROI > 0) {
        cout << "Processing ROI #" << ROIcount + 1 << "...\n";
        cout << "ROI location: (" << pixelX[ROIcount] << "," << pixelY[ROIcount] << ")\n";
        cout << "ROI size: " << sX[ROIcount] << "x" << sY[ROIcount] << "\n";
        cout << "ROI function: althistostretch" << endl;
        /*create histogram before stretching*/
        for (int i = 0; i < src.getNumberOfRows(); i++)
            for (int j = 0; j < src.getNumberOfColumns(); j++)
                if (i >= pixelY[ROIcount] && i < pixelY[ROIcount] + sY[ROIcount] &&
                    j >= pixelX[ROIcount] && j < pixelX[ROIcount] + sX[ROIcount])
                    graylevels[src.getPixel(i, j)]++;
        max = *max_element(graylevels, graylevels + 256);
        cout << "C: " << c[ROIcount] << endl;
        cout << "D: " << d[ROIcount] << endl;
        hist1.resize(256, 256);
        for (int i = 0; i < 256; i++)
            for (int j = 0; j < 256; j++)
                if (i < ((float) graylevels[256 - j] / max) * (float) 256)
                    hist1.setPixel(256 - i, 256 - j, 0);
                else hist1.setPixel(256 - i, 256 - j, 255);
        if (ROIcount == 0) hist1.save("before_althistostretch_ROI1.pgm");
        else if (ROIcount == 1) hist1.save("before_althistostretch_ROI2.pgm");
        else if (ROIcount == 2) hist1.save("before_althistostretch_ROI3.pgm");
        for (int i = 0; i < 256; i++) graylevels[i] = 0;
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
                            (src.getPixel(i, j) - (c[ROIcount]) * 1.05) *
                            ((b - a) / ((d[ROIcount] * 0.95) - (c[ROIcount]) * 1.05))) + a);
        /*create histogram after stretching*/
        for (int i = 0; i < tgt.getNumberOfRows(); i++)
            for (int j = 0; j < tgt.getNumberOfColumns(); j++)
                if (i >= pixelY[ROIcount] && i < pixelY[ROIcount] + sY[ROIcount] &&
                    j >= pixelX[ROIcount] && j < pixelX[ROIcount] + sX[ROIcount])
                    graylevels[tgt.getPixel(i, j)]++;
        max = *max_element(graylevels + 1, graylevels + 255);
        hist2.resize(256, 256);
        for (int i = 0; i < 256; i++)
            for (int j = 0; j < 256; j++)
                if (i < ((float) graylevels[256 - j] / max) * (float) 256)
                    hist2.setPixel(256 - i, 256 - j, 0);
                else hist2.setPixel(256 - i, 256 - j, 255);
        if (ROIcount == 0) hist2.save("after_althistostretch_ROI1.pgm");
        else if (ROIcount == 1) hist2.save("after_althistostretch_ROI2.pgm");
        else if (ROIcount == 2) hist2.save("after_althistostretch_ROI3.pgm");
        for (int i = 0; i < 256; i++) graylevels[i] = 0;
        --numROI;
        ++ROIcount;
    }
    cout << endl;
}

void
utility::histothres(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3], int sY[3], int t[3],
                    int c[3], int d[3]) {
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    int ROIcount = 0;
    while (numROI > 0) {
        cout << "Processing ROI #" << ROIcount + 1 << "...\n";
        cout << "ROI location: (" << pixelX[ROIcount] << "," << pixelY[ROIcount] << ")\n";
        cout << "ROI size: " << sX[ROIcount] << "x" << sY[ROIcount] << "\n";
        cout << "ROI function: histothres" << endl;
        /*apply threshold stretching to roi*/
        if (ROIcount == 0)
            for (int i = 0; i < src.getNumberOfRows(); i++)
                for (int j = 0; j < src.getNumberOfColumns(); j++)
                    tgt.setPixel(i, j, checkValue(src.getPixel(i, j)));
        for (int i = 0; i < src.getNumberOfRows(); i++)
            for (int j = 0; j < src.getNumberOfColumns(); j++)
                if (i >= pixelY[ROIcount] && i < pixelY[ROIcount] + sY[ROIcount] &&
                    j >= pixelX[ROIcount] && j < pixelX[ROIcount] + sX[ROIcount] && src.getPixel(i, j) < t[ROIcount])
                    tgt.setPixel(i, j, checkValue(
                            (src.getPixel(i, j) - (c[ROIcount]) * 1.05) *
                            ((b - a) / ((d[ROIcount] * 0.95) - (c[ROIcount]) * 1.05))) + a);
        --numROI;
        ++ROIcount;
    }
    cout << endl;
}

void utility::percchastrech(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3], int sY[3],
                            int channel[3],
                            int c[3], int d[3]) {
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    int ROIcount = 0;
    while (numROI > 0) {
        cout << "Processing ROI #" << ROIcount + 1 << "...\n";
        cout << "ROI location: (" << pixelX[ROIcount] << "," << pixelY[ROIcount] << ")\n";
        cout << "ROI size: " << sX[ROIcount] << "x" << sY[ROIcount] << "\n";
        cout << "ROI function: percchastrech" << endl;
        /*apply color channel stretching to roi*/
        if (ROIcount == 0)
            for (int i = 0; i < src.getNumberOfRows(); i++)
                for (int j = 0; j < src.getNumberOfColumns(); j++) {
                    tgt.setPixel(i, j, RED, checkValue(src.getPixel(i, j, RED)));
                    tgt.setPixel(i, j, GREEN, checkValue(src.getPixel(i, j, GREEN)));
                    tgt.setPixel(i, j, BLUE, checkValue(src.getPixel(i, j, BLUE)));
                }
        for (int i = 0; i < src.getNumberOfRows(); i++)
            for (int j = 0; j < src.getNumberOfColumns(); j++)
                if (i >= pixelY[ROIcount] && i < pixelY[ROIcount] + sY[ROIcount] &&
                    j >= pixelX[ROIcount] && j < pixelX[ROIcount] + sX[ROIcount]) {
                    if (channel[ROIcount] == RED)
                        tgt.setPixel(i, j, RED, checkValue(
                                (src.getPixel(i, j, RED) - (c[ROIcount]) * 1.05) *
                                ((b - a) / ((d[ROIcount] * 0.95) - (c[ROIcount]) * 1.05))) + a);
                    else if (channel[ROIcount] == GREEN)
                        tgt.setPixel(i, j, GREEN, checkValue(
                                (src.getPixel(i, j, GREEN) - (c[ROIcount]) * 1.05) *
                                ((b - a) / ((d[ROIcount] * 0.95) - (c[ROIcount]) * 1.05))) + a);
                    else if (channel[ROIcount] == BLUE)
                        tgt.setPixel(i, j, BLUE, checkValue(
                                (src.getPixel(i, j, BLUE) - (c[ROIcount]) * 1.05) *
                                ((b - a) / ((d[ROIcount] * 0.95) - (c[ROIcount]) * 1.05))) + a);
                }
        --numROI;
        ++ROIcount;
    }
    cout << endl;
}

void
utility::rgbstretch(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3], int sY[3], int c[3],
                    int d[3]) {
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    int ROIcount = 0;
    while (numROI > 0) {
        cout << "Processing ROI #" << ROIcount + 1 << "...\n";
        cout << "ROI location: (" << pixelX[ROIcount] << "," << pixelY[ROIcount] << ")\n";
        cout << "ROI size: " << sX[ROIcount] << "x" << sY[ROIcount] << "\n";
        cout << "ROI function: rgbstretch" << endl;
        /*apply color channel stretching to roi*/
        if (ROIcount == 0)
            for (int i = 0; i < src.getNumberOfRows(); i++)
                for (int j = 0; j < src.getNumberOfColumns(); j++) {
                    tgt.setPixel(i, j, RED, checkValue(src.getPixel(i, j, RED)));
                    tgt.setPixel(i, j, GREEN, checkValue(src.getPixel(i, j, GREEN)));
                    tgt.setPixel(i, j, BLUE, checkValue(src.getPixel(i, j, BLUE)));
                }
        for (int i = 0; i < src.getNumberOfRows(); i++)
            for (int j = 0; j < src.getNumberOfColumns(); j++)
                if (i >= pixelY[ROIcount] && i < pixelY[ROIcount] + sY[ROIcount] &&
                    j >= pixelX[ROIcount] && j < pixelX[ROIcount] + sX[ROIcount]) {
                    tgt.setPixel(i, j, RED, checkValue((src.getPixel(i, j, RED) - (c[ROIcount]) * 1.05) *
                                                       ((b - a) / ((d[ROIcount] * 0.95) - (c[ROIcount]) * 1.05))) + a);
                    tgt.setPixel(i, j, GREEN, checkValue((src.getPixel(i, j, GREEN) - (c[ROIcount]) * 1.05) *
                                                         ((b - a) / ((d[ROIcount] * 0.95) - (c[ROIcount]) * 1.05))) +
                                              a);
                    tgt.setPixel(i, j, BLUE, checkValue((src.getPixel(i, j, BLUE) - (c[ROIcount]) * 1.05) *
                                                        ((b - a) / ((d[ROIcount] * 0.95) - (c[ROIcount]) * 1.05))) + a);
                }
        --numROI;
        ++ROIcount;
    }
    cout << getRed(getHue(10, 50, 250), getSaturation(10, 50, 250), getIntensity(10, 50, 250)) << endl;
    cout << getGreen(getHue(10, 50, 250), getSaturation(10, 50, 250), getIntensity(10, 50, 250)) << endl;
    cout << getBlue(getHue(10, 50, 250), getSaturation(10, 50, 250), getIntensity(10, 50, 250)) << endl;
    cout << endl;
}

/*-----------------------------------------------------------------------**/
void utility::istretch(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3], int sY[3],
                       int value[3]) {
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
void utility::hstretch(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3], int sY[3],
                       int value[3]) {
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
void utility::sstretch(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3], int sY[3],
                       int value[3]) {
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
void utility::fullhsistretch(image &src, image &tgt, int numROI, int pixelX[3], int pixelY[3], int sX[3], int sY[3],
                             int value[3]) {
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