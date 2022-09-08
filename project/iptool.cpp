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

int main (int argc, char** argv)
{
	image src, tgt;
	FILE *fp;
	char str[MAXLEN];
	char outfile[MAXLEN];
	char *pch;
    int numROI, ROIcount;
    int pixelX[3], pixelY[3], sX[3], sY[3], ws[3], threshold[3], moreC[3];
	if ((fp = fopen(argv[1],"r")) == NULL) {
		fprintf(stderr, "Can't open file: %s\n", argv[1]);
		exit(1);
	}

	while(fgets(str,MAXLEN,fp) != NULL) {
		pch = strtok(str, " ");
		src.read(pch);
		pch = strtok(NULL, " ");
		strcpy(outfile, pch);
		pch = strtok(NULL, " ");
        if (strncasecmp(pch,"add",MAXLEN)==0) {
            numROI = atoi(strtok(NULL, " "));
            cout << "Add function selected for " << numROI << " ROI(s).\n";
            cout << "Resolution: " << src.getNumberOfRows() << "x" << src.getNumberOfColumns() << "\n";
            int ROIremaining = numROI;
            ROIcount = 0;
            int value[3];
            while (ROIremaining > 0)
            {
                cout << "Processing ROI #" << ROIcount + 1 << ".\n";
                pixelX[ROIcount] = atoi(strtok(NULL, " "));
                pixelY[ROIcount] = atoi(strtok(NULL, " "));
                cout << "ROI location: (" << pixelX[ROIcount] << "," << pixelY[ROIcount] << ")\n";
                sX[ROIcount] = atoi(strtok(NULL, " "));
                sY[ROIcount] = atoi(strtok(NULL, " "));
                cout << "ROI size: " << sX[ROIcount] << "x" << sY[ROIcount] << "\n";
                value[ROIcount] = atoi(strtok(NULL, " "));
                cout << "Add value is: " << value[ROIcount] << "\n";
                --ROIremaining;
                ++ROIcount;
            }
            utility::addGrey(src, tgt, numROI, pixelX, pixelY, sX, sY, value);
            cout << "\n";
        }

        else if (strncasecmp(pch,"binarize",MAXLEN)==0) {
            numROI = atoi(strtok(NULL, " "));
            cout << "Binarize function selected for " << numROI << " ROI(s).\n";
            cout << "Resolution: " << src.getNumberOfRows() << "x" << src.getNumberOfColumns() << "\n";
            int ROIremaining = numROI;
            ROIcount = 0;
            bool isDouble[3];
            int threshold1[3];
            int threshold2[3];
            while (ROIremaining > 0)
            {
                cout << "Processing ROI #" << ROIcount + 1 << ".\n";
                pixelX[ROIcount] = atoi(strtok(NULL, " "));
                pixelY[ROIcount] = atoi(strtok(NULL, " "));
                cout << "ROI location: (" << pixelX[ROIcount] << "," << pixelY[ROIcount] << ")\n";
                sX[ROIcount] = atoi(strtok(NULL, " "));
                sY[ROIcount] = atoi(strtok(NULL, " "));
                cout << "ROI size: " << sX[ROIcount] << "x" << sY[ROIcount] << "\n";
                if (atoi(strtok(NULL, " ")) == 1)
                    isDouble[ROIcount] = true;
                else
                    isDouble[ROIcount] = false;
                if (isDouble[ROIcount])
                {
                    threshold1[ROIcount] = atoi(strtok(NULL, " "));
                    threshold2[ROIcount] = atoi(strtok(NULL, " "));
                    cout << "Double threshold values: " << threshold1[ROIcount] << ", " << threshold2[ROIcount] << "\n";
                }
                else
                {
                    threshold1[ROIcount] = atoi(strtok(NULL, " "));
                    cout << "Threshold value: " << threshold1[ROIcount] << "\n";
                }
                --ROIremaining;
                ++ROIcount;
            }
            utility::binarize(src, tgt, numROI, pixelX, pixelY, sX, sY, isDouble, threshold1, threshold2);
            cout << "\n";
        }

/*-----------------------------------------------------------------------
        else if (strncasecmp(pch,"doubleThreshold",MAXLEN)==0) {
            * Image scaling
            pch = strtok(NULL, " ");
            utility::doubleThreshold(src,tgt,atoi(pch));
        }
-----------------------------------------------------------------------**/

		else if (strncasecmp(pch,"scale",MAXLEN)==0) {
			/* Image scaling */
			pch = strtok(NULL, " ");
			utility::scale(src,tgt,atof(pch));
		}

        else if (strncasecmp(pch,"uniformSmoothing",MAXLEN)==0) {
            numROI = atoi(strtok(NULL, " "));
            cout << "Uniform smoothing function selected for " << numROI << " ROI(s).\n";
            cout << "Resolution: " << src.getNumberOfRows() << "x" << src.getNumberOfColumns() << "\n";
            int ROIremaining = numROI;
            ROIcount = 0;
            while (ROIremaining > 0)
            {
                cout << "Processing ROI #" << ROIcount + 1 << ".\n";
                pixelX[ROIcount] = atoi(strtok(NULL, " "));
                pixelY[ROIcount] = atoi(strtok(NULL, " "));
                cout << "ROI location: (" << pixelX[ROIcount] << "," << pixelY[ROIcount] << ")\n";
                sX[ROIcount] = atoi(strtok(NULL, " "));
                sY[ROIcount] = atoi(strtok(NULL, " "));
                cout << "ROI size: " << sX[ROIcount] << "x" << sY[ROIcount] << "\n";
                ws[ROIcount] = atoi(strtok(NULL, " "));
                cout << "Window size: " << ws[ROIcount] << "\n";
                --ROIremaining;
                ++ROIcount;
            }
            utility::uniformSmoothing(src,tgt, numROI, pixelX, pixelY, sX, sY, ws);
            cout << "\n";
        }

        else if (strncasecmp(pch,"adaptiveSmoothing",MAXLEN)==0) {
            numROI = atoi(strtok(NULL, " "));
            cout << "Adaptive smoothing function selected for " << numROI << " ROI(s).\n";
            cout << "Resolution: " << src.getNumberOfRows() << "x" << src.getNumberOfColumns() << "\n";
            int ROIremaining = numROI;
            ROIcount = 0;
            while (ROIremaining > 0)
            {
                cout << "Processing ROI #" << ROIcount + 1 << ".\n";
                pixelX[ROIcount] = atoi(strtok(NULL, " "));
                pixelY[ROIcount] = atoi(strtok(NULL, " "));
                cout << "ROI location: (" << pixelX[ROIcount] << "," << pixelY[ROIcount] << ")\n";
                sX[ROIcount] = atoi(strtok(NULL, " "));
                sY[ROIcount] = atoi(strtok(NULL, " "));
                cout << "ROI size: " << sX[ROIcount] << "x" << sY[ROIcount] << "\n";
                ws[ROIcount] = atoi(strtok(NULL, " "));
                cout << "Window size: " << ws[ROIcount] << "\n";
                threshold[ROIcount] = atoi(strtok(NULL, " "));
                cout << "Threshold value: " << threshold[ROIcount] << "\n";
                --ROIremaining;
                ++ROIcount;
            }
            utility::adaptiveSmoothing(src, tgt, numROI, pixelX, pixelY, sX, sY, ws, threshold);
            cout << "\n";
        }

        else if (strncasecmp(pch,"moreColor",MAXLEN)==0) {
            pch = strtok(NULL, " ");
            utility::moreColor(src, tgt, numROI, pixelX, pixelY, sX, sY, moreC);
        }

		else {
			printf("No function: %s\n", pch);
			continue;
		}

		tgt.save(outfile);
	}
	fclose(fp);
	return 0;
}

