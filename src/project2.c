/*
  This is an example of reading and writing an image using the ppmIO.c
  routines.  To manipulate the image, change the pixel color values.
*/

#include <stdio.h>
#include <stdlib.h>
#include "ppmIO.h"
#include "vision.h"

#define USECPP 0

int main(int argc, char *argv[]) {
  Pixel *image;
  int rows, cols, colors;
  long imagesize;
  long i;
  int gain = atoi(argv[3]);

  if(argc < 4) {
    printf("Usage: ./project2 <input file> <output file> <threshold>\n");
    exit(-1);
  }

  /* read in the image */
  image = readPPM(&rows, &cols, &colors, argv[1]);
  if(!image) {
    fprintf(stderr, "Unable to read %s\n", argv[1]);
    exit(-1);
  }

  /* calculate the image size */
  imagesize = (long)rows * (long)cols;

  /* mess with the image here   */
  for(i=0; i<imagesize; i++) {
    if (image[i].r > (gain * image[i].g)) {
      image[i].r = 255;
      image[i].g = 0;
      image[i].b = 0;
    }
    else {
      image[i].r = 0;
      image[i].b = 0;
      image[i].g = 0;
    }
  } 

  
  /* write out the resulting image */
  writePPM(image, rows, cols, colors /* s/b 255 */, argv[2]);

  /* free the image memory */
#if USECPP
  delete[] image;
#else
  free(image);
#endif

  return(0);
}
