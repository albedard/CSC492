#ifndef	FILE_IO_TGA_H
#define	FILE_IO_TGA_H

#include "fileIO.h"


//Image struct has a pointer to the data,
//the number of rows, number of columns, and number of pixels
//along with the head
typedef struct {

	unsigned char *theData;
	int   nbrRows, nbrCols;
	int		numberOfPixels;
	char	head[18];


} image;



image* picture;

unsigned char *readTGA(char *fileName, eImageFileType *theType);
//unsigned char *readTGA(char *fileName, int *nbCols, int *nbRows, eImageFileType *theType);
int writeTGA(char *fileName, image picture);

//The image struct for sub image returns ptr to struct
//write image row by row
//get image from image. use arrayutils to move ptr to where it needs to be.



#endif
