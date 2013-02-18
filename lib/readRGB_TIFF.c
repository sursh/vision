#include <stdio.h>
#include <sys/types.h>
#include <tiffio.h>

/*
  This routine reads in a TIFF image from "filename" and returns
  the number of columns and number of rows as arguments. The return
  value of the function is a pointer to the image data, which is
  in ABGR format, with 8 bits per color.  The function returns a 
  NULL value if it is unable to read in the data.
  */
uint32 *tiff_read2raster(char filename[], uint32 *nrow, uint32 *ncol);
uint32 *tiff_read2raster(char filename[], uint32 *nrow, uint32 *ncol) {
  TIFF* tif = TIFFOpen(filename, "r");
  *ncol = *nrow = 0;
  if (tif) {
    uint32 w, h;
    size_t npixels;
    uint32 *raster;

    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
    npixels = w * h;
    raster = (uint32*) _TIFFmalloc(npixels * sizeof (uint32));
    if (raster != NULL) {
      if (TIFFReadRGBAImage(tif, w, h, raster, 0)) {
	*ncol = w;
	*nrow = h;
	TIFFClose(tif);
	return(raster);
      }
      else {
	TIFFClose(tif);
	return(NULL);
      }
    }
    else {
      TIFFClose(tif);
      return(NULL);
    }
  }
  else {
    return(NULL);
  }
}



