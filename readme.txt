This software is architectured as follows. This software can work on grad server.

iptools -This folder hosts the files that are compiled into a static library. 
	image - This folder hosts the files that define an image.
	utility- this folder hosts the files that students store their implemented algorithms.
	
lib- This folder hosts the static libraries associated with this software.

project- This folder hosts the files that will be compiled into executables.
	bin- This folder hosts the binary executables created in the project directory.



*** INSTALATION ***

On Linux

Enter the project directory in terminal and run make

As a result you should get iptool in project/bin directory.

*** FUNCTIONS ***

1. Increase/decrease grayscale value: add
Increase/decrease the intensity for a gray-level image.

2. Binarize an image: binarize
Binarize the pixels with the threshold.

3. Apply uniform smoothing: uniform smooth
Smooth pixels uniformly.

4. Apply adaptive smoothing: adptvsmooth
Smooth pixels under a threshold.

5. Scale RGB values: mulcolorbright
Scale RGB values of a pixel.

*** PARAMETERS FILE ***

There are five types of parameters:
1. the input file name;
2. the output file name;
3. the number of ROI; 
4. for each ROI:
   a. starting location (x and y values)
   b. dimensions (width and height values)
   c. function name
5. function specific:
   a. add: value
   b. binarize: threshold
   c. uniformsmooth: window size 
   a. adptvsmooth: window size, threshold
   b. mulcolorbright: red scalar, green scalar, blue scalar

*** Run the program: ./iptool parameters.txt
