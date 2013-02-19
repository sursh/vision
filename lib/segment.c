/*
  This program is an implementation of a 2-pass segmentation algorithm
  on a binary unsigned char image (0 | not 0)

  Arguments:

  image      - pointer to an unsigned char image (8-bits/pixel)
               The image is treated as "binary" (0, not 0)
  rows       - number of rows in the image
  cols       - number of columns in the image
  sizeThresh - minimum size for a region to be returned (regions small than this are ignored)
  location   - a pre-allocated Point array at least VISION_MAX_LOCATIONS big in which the program returns the centroids of the regions found
  bbox       - a pre-allocated Box array at least VISION_MAX_LOCATIONS big in which the program returns the bounding boxes of the regions found
  size       - a pre-allocated long array at least VISION_MAX_LOCATIONS big in which the program returns the size of each region found
  regmap     - a pre-allocated short array that is the size of the image in which the program returns the region map (image with region ids instead of pixel values)

 The function returns the number of regions found, up to VISION_MAX_LOCATIONS.
  
*/

#include <stdlib.h>
#include <stdio.h>
#include "computerVision.h"

// set this to 1 if you want to print out intermediate information
#define DEBUG 1

int locateRegions(unsigned char *image, const long rows, const long cols, const long sizeThresh, int MaxLocations, Point_t location[], Box_t bbox[], long size[], short *regmap) {
  // statics to reduce setup time
  static long *boundto = NULL;
  static long *regcard = NULL;
  //  static short *regmap = NULL;  /* passed in to the function pre-allocated */
  static long lastImageSize = 0;

  // local variables
  const long imageSize = rows * cols;
  short *regptr;
  long i, j, k;
  long *bt, *reg;
  unsigned char backpix, uppix, curpix;
  long nRegions=0;
  unsigned char *tb;
  unsigned char *tbu;
  unsigned long upID, backID;
  long numLocations;

  long *sizeid = malloc(sizeof(long) * MaxLocations);

  // if the image size changed, then we need to re-allocate
  if(lastImageSize != imageSize) {
    if(boundto)
      free(boundto);
    if(regcard)
      free(regcard);
    //    if(regmap)
    //      free(regmap);

    boundto = NULL;
    regcard = NULL;
    //    regmap = NULL;

    lastImageSize = imageSize;
  }
    
  // allocate the static variables if necessary
  if(boundto == NULL)
    boundto = malloc(sizeof(long) * imageSize/2);
  if(regcard == NULL)
    regcard = malloc(sizeof(unsigned long) * imageSize/2);
  //  if(regmap == NULL)
  //    regmap = malloc(sizeof(short) * imageSize);

  // initialize the region map and boundto variables
#if DEBUG
  printf("Initializing regions\n");
#endif
  reg = regcard;
  bt = boundto;
  for(i=0;i<imageSize/2;i++, reg++, bt++) {
    *reg = 0;
    *bt = i;
  }

  // initialize the parts of the image we won't use
  tb = image;
  regptr = regmap;
  for(i=0;i<cols;i++, tb++, regptr++) {
    *tb = 0x0000;
    *regptr = -1;
  }

  tb = image;
  regptr = regmap;
  for(i=0;i<rows;i++, tb+=cols, regptr+=cols) {
    *tb = 0x0000;
    *regptr = -1;
  }

#if DEBUG
  printf("Starting 2-pass algorithm\n");
#endif

  // segment the image using a 2-pass algorithm (4-connected)
  tb = &(image[cols+1]);
  tbu = &(image[1]);
  regptr = &(regmap[cols+1]);
  for(i=0;i<rows-1;i++,tb++, tbu++, regptr++) {
    backpix = *(tb-1);
    for(j=0;j<cols-1;j++, tb++, tbu++, regptr++) {

      // get the value of the current pixel
      curpix = *tb;

      // set the default value of regptr
      *regptr = -1;

      // if the pixel is in the region
      if(curpix) {

	// get the up pixel 
	uppix = *tbu;

	// test if the rgb values are similar for the up and back pixels
	if(uppix) {
	  // similar to up pixel
	  if(backpix) {
	    // similar to back pixel as well so get the region IDs
	    upID = *(regptr - cols);
	    backID = *(regptr - 1);
	    if(backID == upID) { // equal region ids
	      *regptr = upID;
	    }
	    else { // not equal region ids
	      if(boundto[upID] < boundto[backID]) {
		*regptr = boundto[upID];
		boundto[backID] = boundto[upID];
	      }
	      else {
		*regptr = boundto[backID];
		boundto[upID] = boundto[backID];
	      }
	    }
	  }
	  else {
	    // similar only to the top pixel
	    *regptr = *(regptr - cols);
	  }
	}
	else if(backpix) {
	  // similar only to back pixel
	  *regptr = *(regptr - 1);
	}
	else {
	  // not similar to either pixel
	  *regptr = nRegions++;
	}
      }

      backpix = curpix;

    }
  }

  // get out if there's nothing else to do
  if(nRegions == 0) {
#if DEBUG
    printf("No regions\n");
#endif
    return(0);
  }

#if DEBUG
  printf("Fixing IDs\n");
#endif
  // second pass, fix the IDs and calculate the region sizes
  regptr = regmap;
  for(i=0;i<imageSize;i++, regptr++) {
    if(*regptr >= 0) {
      *regptr = boundto[*regptr];

      // need to follow the tree in some special cases
      while(boundto[*regptr] != *regptr)
	*regptr = boundto[*regptr];

      regcard[*regptr]++;
    }
  }

#if DEBUG
  printf("Calculating the N largest regions\n");
#endif
  // grab the N largest ones
  for(i=0;i<MaxLocations && i<nRegions;i++) {
    size[i] = 0;
    sizeid[i] = -1;
    for(j=0;j<nRegions;j++) {

      // don't consider regions already in the list
      for(k=0;k<i;k++) {
	if(j == sizeid[k])
	  break;
      }
      if(k < i)
	continue;

      if((regcard[j] > sizeThresh) && (regcard[j] > size[i])) {
	size[i] = regcard[j];
	sizeid[i] = j;
      }
    }
    if(size[i] == 0) {
      break;
    }

    // do this so we can fix the id values below
    boundto[sizeid[i]] = -2;
  }

  numLocations = i;

#if DEBUG
  printf("Calculating new region ids\n");
#endif

  // calculate new ids for the regions in order of size
  for(i=0,j=0;i<nRegions;i++) {
    if(boundto[i] == -2) {
      boundto[i] = j;
      j++;
    }
    else
      boundto[i] = -1;
  }


#if DEBUG
  printf("Calculating centroids for %ld regions\n", numLocations);
  for(i=0;i<numLocations;i++) {
    printf("id %ld size %ld\n", sizeid[i], size[i]);
  }
#endif

  // now calculate centroids and bounding boxes

  // initialize the bounding boxes and centroids
  for(i=0;i<numLocations;i++) {
    location[i][0] = location[i][1] = 0;
    bbox[i][0] = bbox[i][1] = 100000;
    bbox[i][2] = bbox[i][3] = 0;
    size[i] = 0;
  }

  // one pass through the image to calculate region sizes, bounding boxes, and centroids
  regptr = regmap;
  for(j=0;j<rows;j++) {
    for(k=0;k<cols;k++, regptr++) {
      // assign the new region id to the region map
      if(*regptr >= 0) {
	*regptr = boundto[*regptr];
	if(*regptr < 0) 
	  continue;

	location[*regptr][0] += k;
	location[*regptr][1] += j;

	bbox[*regptr][0] = j < bbox[*regptr][0] ? j : bbox[*regptr][0];
	bbox[*regptr][1] = k < bbox[*regptr][1] ? k : bbox[*regptr][1];
	bbox[*regptr][2] = j > bbox[*regptr][2] ? j : bbox[*regptr][2];
	bbox[*regptr][3] = k > bbox[*regptr][3] ? k : bbox[*regptr][3];

	// recalculate size
	size[*regptr]++;
      }
    }
  }

  for(i=0;i<numLocations;i++) {
    location[i][0] /= size[i];
    location[i][1] /= size[i];
  }
      
#if DEBUG
  printf("Terminating normally\n");
#endif

  return(numLocations);
}
