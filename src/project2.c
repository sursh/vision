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

void grow(unsigned char *mask, unsigned char *grown, int rows, int cols);

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

  if(argc < 5) {
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

  /* make copies of mask to use in image implementation */
  //memcpy(grown, mask, imagesize);

  /* manipulate the images */
  grow(mask, grown, rows, cols);

  /* write out the files */
  writePGM(mask, rows, cols, intensities, maskfile);
  writePGM(grown, rows, cols, intensities, processedfile);

  /* free the image memory */
  free(image);
  free(mask);
  free(grown);

  return(0);
}

/* iterates through mask and outputs grown */
void grow(unsigned char *mask, unsigned char *grown, int rows, int cols) {

  long imagesize, i;
  int x, y, j, k;

  imagesize = (long)rows * (long)cols;

  for (i=0; i < imagesize; ++i){

    /* don't process the 1 pixel layer around the whole image */
    if ((i < (cols)) || i > (imagesize - cols) || (i % cols) == 0 || (i % cols) == (cols-1)) {
      grown[i] = mask[i];
    } 
    /* ignore if foreground */
    else if (mask[i] == 0) {
      grown[i] = mask[i];
    }
    /* if background, apply growing algorithm */
    else if (mask[i] == 255) {
      x = i % cols;
      y = i / cols;
    }
    else {
      grown[i] = mask[i];
    }
  }
  
}





















