/*
  This is an example of reading and writing an image using the ppmIO.c
  routines.  To manipulate the image, change the pixel color values.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ppmIO.h"
#include "vision.h"

#define USECPP 0
#define intensities 255

void grow(unsigned char *image, int rows, int cols);

int main(int argc, char *argv[]) {
  Pixel *image;
  int rows, cols, colors;
  long imagesize;
  long i;
  int gain;
  unsigned char *mask = NULL;
  unsigned char *grown = NULL;
  char *inputfile;
  char *maskfile;
  char *processedfile;

  if(argc < 4) {
    printf("Usage: ./project2 <input file> <mask file> <altered file> <gain>\n");
    exit(-1);
  }

  inputfile = argv[1];
  maskfile = argv[2];
  processedfile = argv[3];
  gain = atoi(argv[4]);

  /* read in the image */
  image = readPPM(&rows, &cols, &colors, inputfile);
  if(!image) {
    fprintf(stderr, "Unable to read %s\n", inputfile);
    exit(-1);
  }

  /* calculate the image size */
  imagesize = (long)rows * (long)cols;
  mask = (unsigned char*)malloc(sizeof(unsigned char) * imagesize);
  grown = (unsigned char*)malloc(sizeof(unsigned char) * imagesize);

  /* mess with the image here   */
  for(i=0; i<imagesize; i++) {
    if (image[i].r > (gain * image[i].g)) {
      mask[i] = 255;
    }
    else {
      mask[i] = 0;
    }
  } 

  /* make a copy of mask */
  memcpy(grown, mask, imagesize);

  /* write out the mask */
  writePGM(mask, rows, cols, intensities, maskfile);

  /* grow the regions and print out resulting file */
  grow(grown, rows, cols);
  writePGM(grown, rows, cols, intensities, processedfile);

  /* free the image memory */
  free(image);
  free(mask);
  free(grown);

  return(0);
}

void grow(unsigned char *image, int rows, int cols) {

  long imagesize, i;
  imagesize = (long)rows * (long)cols;

  /* arbitrary image manipulation for testing */
  for (i=0; i < imagesize; ++i){
    if (image[i] == 255) image[i] = 128;
  }
  
}
