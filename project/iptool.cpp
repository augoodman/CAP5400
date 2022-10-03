/************************************************************
 *															*
 * This sample project include three functions:				*
 * 1. Add intensity for gray-level image.					*
 *    Input: source image, output image name, value			*
 *															*
 * 2. Image thresholding: pixels will become black if the	*
 *    intensity is below the threshold, and white if above	*
 *    or equal the threshold.								*
 *    Input: source image, output image name, threshold		*
 *															*
 * 3. Image scaling: reduction/expansion of 2 for 			*
 *    the width and length. This project uses averaging 	*
 *    technique for reduction and pixel replication			*
 *    technique for expansion.								*
 *    Input: source image, output image name, scale factor	*
 *															*
 ************************************************************/

#include "../iptools/core.h"
#include <strings.h>
#include <string.h>

using namespace std;

#define MAXLEN 256

int main(int argc, char **argv) {
    image src, tgt;
    FILE *fp;
    char str[MAXLEN];
    char outfile[MAXLEN];
    char *pch;
    int numROI, ROIcount;
    int pixelX[3], pixelY[3], sX[3], sY[3], p1[3], p2[3];
    float dr[3], dg[3], db[3];
    if ((fp = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Can't open file: %s\n", argv[1]);
        exit(1);
    }

    while (fgets(str, MAXLEN, fp) != NULL) {
        pch = strtok(str, " ");
        src.read(pch);
        cout << "Input file: " << pch << "\n";
        cout << "Resolution: " << src.getNumberOfRows() << "x" << src.getNumberOfColumns() << "\n";
        pch = strtok(NULL, " ");
        strcpy(outfile, pch);
        cout << "Output file: " << pch << "\n";
        numROI = atoi(strtok(NULL, " "));
        cout << "Processing " << numROI << " ROI(s) total.\n\n";
        int ROIremaining = numROI;
        ROIcount = 0;
        while (ROIremaining > 0) {
            cout << "Processing ROI #" << ROIcount + 1 << "...\n";
            pixelX[ROIcount] = atoi(strtok(NULL, " "));
            pixelY[ROIcount] = atoi(strtok(NULL, " "));
            cout << "ROI location: (" << pixelX[ROIcount] << "," << pixelY[ROIcount] << ")\n";
            sX[ROIcount] = atoi(strtok(NULL, " "));
            sY[ROIcount] = atoi(strtok(NULL, " "));
            cout << "ROI size: " << sX[ROIcount] << "x" << sY[ROIcount] << "\n";
            pch = strtok(NULL, " ");
            if (strncasecmp(pch, "add", MAXLEN) == 0) {
                cout << "ROI function: " << pch << "\n";
                p1[ROIcount] = atoi(strtok(NULL, " "));
                cout << "Add value is: " << p1[ROIcount] << "\n\n";
                utility::add(src, tgt, numROI, pixelX, pixelY, sX, sY, p1);
            } else if (strncasecmp(pch, "binarize", MAXLEN) == 0) {
                cout << "ROI function: " << pch << "\n";
                p1[ROIcount] = atoi(strtok(NULL, " "));
                cout << "Threshold value: " << p1[ROIcount] << "\n\n";
                utility::binarize(src, tgt, numROI, pixelX, pixelY, sX, sY, p1);
            } else if (strncasecmp(pch, "uniformsmooth", MAXLEN) == 0) {
                cout << "ROI function: " << pch << "\n";
                p1[ROIcount] = atoi(strtok(NULL, " "));
                cout << "Window size: " << p1[ROIcount] << "\n\n";
                utility::uniformsmooth(src, tgt, numROI, pixelX, pixelY, sX, sY, p1);
            } else if (strncasecmp(pch, "adptvsmooth", MAXLEN) == 0) {
                cout << "ROI function: " << pch << "\n";
                p1[ROIcount] = atoi(strtok(NULL, " "));
                cout << "Window size: " << p1[ROIcount] << "\n";
                p2[ROIcount] = atoi(strtok(NULL, " "));
                cout << "Threshold value: " << p2[ROIcount] << "\n\n";
                utility::adptvsmooth(src, tgt, numROI, pixelX, pixelY, sX, sY, p1, p2);
            } else if (strncasecmp(pch, "mulcolorbright", MAXLEN) == 0) {
                cout << "ROI function: " << pch << "\n";
                dr[ROIcount] = atof(strtok(NULL, " "));
                cout << "Red channel multiplier: " << dr[ROIcount] << "\n";
                dg[ROIcount] = atof(strtok(NULL, " "));
                cout << "Green channel multiplier: " << dg[ROIcount] << "\n";
                db[ROIcount] = atof(strtok(NULL, " "));
                cout << "Blue channel multiplier: " << db[ROIcount] << "\n";
                utility::mulcolorbright(src, tgt, numROI, pixelX, pixelY, sX, sY, dr, dg, db);
                cout << "\n";
            } else {
                printf("No function: %s\n", pch);
                continue;
            }
            --ROIremaining;
            ++ROIcount;
        }
        tgt.save(outfile);
    }
    fclose(fp);
    return 0;
}

