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
#define FOREGROUND 255
#define BACKGROUND 0

void grow(unsigned char *mask, unsigned char *grown, int rows, int cols);
void shrink(unsigned char *mask, unsigned char *shrunk, int rows, int cols);

int main(int argc, char *argv[]) {

  Pixel *image;
  int rows, cols, colors;
  long imagesize;
  long i;
  int gain;
  unsigned char *mask = NULL;
  unsigned char *grown = NULL;
  unsigned char *shrunk = NULL;
  char *inputfile;
  char *maskfile;
  char *growfile;
  char *shrinkfile;

  if(argc != 2) {
    printf("Usage: ./project2 <input file>\n");
    exit(-1);
  }

  inputfile = argv[1];
  maskfile = "masked.pgm";
  growfile = "grown.pgm";
  gain = 2;
  shrinkfile = "shrunk.pgm";

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
  shrunk = (unsigned char*)malloc(sizeof(unsigned char) * imagesize);

  /* mask the image */
  for(i=0; i<imagesize; i++) {
    if (image[i].r > (gain * image[i].g)) {
      mask[i] = BACKGROUND;
    }
    else {
      mask[i] = FOREGROUND;
    }
  } 

  /* manipulate the images */
  grow(mask, grown, rows, cols);
  shrink(mask, shrunk, rows, cols);

  /* write out the files */
  writePGM(mask, rows, cols, intensities, maskfile);
  writePGM(grown, rows, cols, intensities, growfile);
  writePGM(shrunk, rows, cols, intensities, shrinkfile);

  /* free the image memory */
  free(image);
  free(mask);
  free(grown);
  free(shrunk);

  return(0);
}

/* turn on any background pixel with a foreground pixel neighbor (8-connected) */
void grow(unsigned char *mask, unsigned char *grown, int rows, int cols) {

  long imagesize, i, neighbor;
  int x, y, j, k;

  imagesize = (long)rows * (long)cols;

  for (i=0; i < imagesize; ++i){

    /* don't process the 1 pixel layer around the whole image */
    if ((i < (cols)) || i > (imagesize - cols) || (i % cols) == 0 || (i % cols) == (cols-1)) {
      grown[i] = mask[i];
    } 
    /* ignore if foreground */
    else if (mask[i] == FOREGROUND) {
      grown[i] = mask[i];
    }
    /* if background, apply growing algorithm */
    else if (mask[i] == BACKGROUND) {
      x = i % cols;
      y = i / cols;

      /* look at 8 nearest neighbors. If any are foreground, turn pixel on */
      for (j = -1; j <= 1; ++j) {
        for(k = -1; k <= 1; ++k) {

          neighbor = ( (x+j) + (cols*(y+k)) );

          if (i<200000 && mask[neighbor] == FOREGROUND) printf("i %ld, (%d, %d), j %ld, mask[j] %d\n", i, j, k, (neighbor), mask[neighbor]);

          if (mask[neighbor] == FOREGROUND) {
            grown[i] = FOREGROUND;
          }
          else {
            
          }
        }
      }
    }  
    
    else {
      grown[i] = mask[i];
    }
  }
  
}

/* turn off any foreground pixel with a background pixel neighbor (8-connected) */
void shrink(unsigned char *mask, unsigned char *shrunk, int rows, int cols) {

  long imagesize, i;
  int x, y, j, k;

  imagesize = (long)rows * (long)cols;

  for (i=0; i < imagesize; ++i){

    /* don't process the 1 pixel layer around the whole image */
    if ((i < (cols)) || i > (imagesize - cols) || (i % cols) == 0 || (i % cols) == (cols-1)) {
      shrunk[i] = mask[i];
    } 
    /* ignore if background */
    else if (mask[i] == BACKGROUND) {
      shrunk[i] = mask[i];
    }
    /* if foreground with bg neighbors, turn off */
    else if (mask[i] == FOREGROUND) {

      x = i % cols;
      y = i / cols;

      for (j = -1; j <= 1; ++j) {
        for(k = -1; k <= 1; ++k) {

          /* grab pixel at ((x+j), (y+k)) */
          if (mask[ (cols*(y+k)) + (x+j) ] == BACKGROUND) {
            shrunk[i] = BACKGROUND;
          }
          else {
            shrunk[i] = 128;
          }
        }
      }
    }  
    
    else {
      shrunk[i] = mask[i];
    }
  }
  
}



















