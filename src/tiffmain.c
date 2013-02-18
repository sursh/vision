#include <stdio.h>
#include <sys/types.h>
#include <tiffio.h>

uint32 *tiff_read2raster(char filename[], uint32 *nrow, uint32 *ncol);
int tiff_raster2file(char filename[], uint32 *raster, uint32 nrow, uint32 ncol);

int main(int argc, char *argv[]) {
  uint32 *image;
  uint32 rows, cols;
  int i, j;

  printf("Beginning process\n");

  image = tiff_read2raster(argv[1], &rows, &cols);

  printf("Image is %ld rows by %ld columns\n", rows, cols);

  /* mess with image here */
  for(i=0;i<10;i++) {
    for(j=0;j<10;j++) {
      image[i*cols + j] = 0x00FFFFFF;
    }
  }

  printf("Starting write process\n");

  tiff_raster2file("tmp.tif", image, rows, cols);

  printf("Exiting\n");

  return(0);
}
