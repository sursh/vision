unsigned char *gimage;
Pixel *cimage;

// allocate the images
gimage = malloc(sizeof(unsigned char) * rows * cols);

cimage = malloc(sizeof(Pixel) * rows * cols);

// manipulate the images here
gimage[ r * cols + c] = 16;

cimage[ r * cols + c].r = 128;
cimage[ r * cols + c].g = 100;
cimage[ r * cols + c].b = 200;

// free the images
free(cimage);
free(gimage);