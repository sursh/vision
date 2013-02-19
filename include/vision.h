/*
  Computer Vision include file
*/

#ifndef ComputerVision_h
#define ComputerVision_h


// type definitions
typedef long Point_t[3];	// x y z (or just x y)
typedef char Color_t[3];	// r g b
typedef long Box_t[4];		// top/left/bottom/right
typedef long ColorBound_t[6];	// r hi, r low / g hi, g low / b hi, b low

int locateRegions(unsigned char *image, const long rows, const long cols, const long sizeThresh, int MaxLocations, Point_t location[], Box_t bbox[], long size[], short *regmap);

#endif















