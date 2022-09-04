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
    int pixelX[3], pixelY[3], sX[3], sY[3], ws[3];
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
            cout << "Resolution: " << src.getNumberOfRows() << "x" << src.getNumberOfColumns() << "\n";
            numROI = atoi(strtok(NULL, " "));
            int ROIremaining = numROI;
            ROIcount = 0;
            int value[3];
            cout << "Add function selected for " << numROI << " ROI(s).\n";
            while (ROIremaining > 0)
            {
                cout << "Processing ROI #" << ROIcount+1 << ".\n";
                pixelX[ROIcount] = atoi(strtok(NULL, " "));
                pixelY[ROIcount] = atoi(strtok(NULL, " "));
                cout << "Pixel location: (" << pixelX[ROIcount] << "," << pixelY[ROIcount] << ")\n";
                sX[ROIcount] = atoi(strtok(NULL, " "));
                sY[ROIcount] = atoi(strtok(NULL, " "));
                cout << "Window size: " << sX[ROIcount] << "x" << sY[ROIcount] << "\n";
                value[ROIcount] = atoi(strtok(NULL, " "));
                cout << "Add value is: " << value[ROIcount] << "\n";
                --ROIremaining;
                ++ROIcount;
            }
            utility::addGrey(src, tgt, numROI, pixelX, pixelY, sX, sY, value);
        }

        else if (strncasecmp(pch,"binarize",MAXLEN)==0) {
			/* Thresholding */
			pch = strtok(NULL, "\n");
            utility::binarize(src,tgt,atoi(pch));
        }

        
        else if (strncasecmp(pch,"doubleThreshold",MAXLEN)==0) {
            /* Image scaling */
            pch = strtok(NULL, " ");
            utility::doubleThreshold(src,tgt,atoi(pch));
        }
        
		else if (strncasecmp(pch,"scale",MAXLEN)==0) {
			/* Image scaling */
			pch = strtok(NULL, " ");
			utility::scale(src,tgt,atof(pch));
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

