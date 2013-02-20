Assignment 1: Colony Counting
http://cs.colby.edu/courses/F07/cs397/labs/lab02/

Due 2 October 2007
Overview

For this assignment we will be designing an algorithm for counting colonies on an agar plate. The colonies are of different sizes, mostly round, mostly separated on the plate, and reasonably homogeneous in color.



Setup

Download the data files from here. Note that some of the images are taken with the agar plate facing down, some with it facing up. You don't need to be able to process both. Pick an orientation and stick with it. On the down-facing plates, the colonies will tend to look flatter than they do on the up-facing places.

You will also want to download the files:

segment.c: provides a connected components algorithm.
vision.h: provides a few useful data types.
Put the .c file in your lib directory and add it to the list of files in the makefile there. Put the .h file in your include directory. You will also want to add an include statement to your programs to include vision.h in them. Feel free to modify vision.h to incorporate any work that you do.

Procedure

The colony pictures for this assignment were taken on a red background. Using a suitable thresholding method, separate the colonies from the background. Thresholds on the green and blue channels will likely work well enough. The output of this step should be a binary mask. Represent the mask as an array (image) of unsigned chars. You can write out the mask to a file using the writePGM() function. If you use 0 for the background and 255 for the foreground then you will be able to visualize the mask easily.
Write the following functions. Each should take as an argument an unsigned char array (input image), the number of rows, the number of columns, and an unsigned char array (output image) to hold the output.
Grow: executes either 4 or 8 connected growing (your choice). If you want, you can add a parameter to indicate the number of grow operations to execute.
Shrink: executes either 4 or 8 connected shrinking (should be the opposite of your choice for growing). If you wish, add a parameter to indicate the number of shrinking operations to execute.
Median: executes a median filter on the input image.