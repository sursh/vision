#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <tiffio.h>

/*
  This routine writes an RGB TIFF image to "filename" scanline by scanline.
  The arguments to the function are the filename, the raster data in ABGR
  format (8 bits per color), the number of rows, and the number of columns.
  This function automatically builds the TIFF information for a single RGB
  image of the given size.
  */

#define INTEL 1

int tiff_raster2file(char filename[], uint32 *raster, uint32 nrow, uint32 ncol);
int tiff_raster2file(char filename[], uint32 *raster, uint32 nrow, uint32 ncol) {
  int i,j;
  long stripsperimage = 0;
  long rasterbase;
  unsigned char *tmpStorage;
  unsigned char *tmpPtr;
  TIFF* tif = TIFFOpen(filename, "w");
  if (tif) {
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, ncol);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, nrow);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 3);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, 1); /* none */
    TIFFSetField(tif, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);
    TIFFSetField(tif, TIFFTAG_XRESOLUTION, 1200.0);
    TIFFSetField(tif, TIFFTAG_YRESOLUTION, 1200.0);
    TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);

    /* calculate the number of strips we want in the TIFF file */
    stripsperimage = nrow * ncol / 8000;
    if(stripsperimage < 1)
      stripsperimage = 1;
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, stripsperimage);

    tmpStorage = (unsigned char *)malloc(sizeof(unsigned char) * ncol * 3);
    if(!tmpStorage) {
      fprintf(stderr, "Memory allocation error\n");
      TIFFClose(tif);
      return(-1);
    }
    if (raster != NULL) {
      for(i=0;i<nrow;i++) {
	rasterbase = (nrow - i - 1) * ncol;
	for(j=0;j<ncol*3;) {
	  tmpPtr = (unsigned char *)&(raster[rasterbase]);
#if INTEL
	  tmpStorage[j] = tmpPtr[0];
	  j++;
	  tmpStorage[j] = tmpPtr[1];
	  j++;
	  tmpStorage[j] = tmpPtr[2];
	  j++;
#else
	  tmpStorage[j] = tmpPtr[3];
	  j++;
	  tmpStorage[j] = tmpPtr[2];
	  j++;
	  tmpStorage[j] = tmpPtr[1];
	  j++;
#endif
	  rasterbase++;
	}
	if (TIFFWriteScanline(tif, tmpStorage, i, 0) != 1) {
	  fprintf(stderr, "Unable to write scanline %d\n",i);
	  return(-1);
	}
      }

      TIFFClose(tif);
      return(0);
    }
    else {
      TIFFClose(tif);
      fprintf(stderr, "image is NULL\n");
      return(-1);
    }
  }
  else {
    fprintf(stderr, "Unable to open %s for writing\n",filename);
    return(-1);
  }
}



