//
//  main.c
//  Read TGA
//
//  Created by Jean-Yves Hervé [Local] on 2016-02-03.
//  Copyright © 2016 Jean-Yves Hervé. All rights reserved.
//

#include <stdio.h>
//
#include "fileIO_TGA.h"

typedef	struct sTextureInfoStruct {
	short	nbCols,
		nbRows,
		bytesPerPixel;
} sTextureInfo;

#define	kNameFile1	"C:/Users/Amanda/Desktop/prog01/Prog 01/Handouts/Read TGA/Images/claypots.tga"
#define	kNameFile2	"C:/Users/Amanda/Desktop/prog01/Prog 01/Handouts/Read TGA/Output/out_bottles.tga"




int main(int argc, const char * argv[]) {

	sTextureInfo	gMyTextInfo;
	int nbCols = NULL;
	int nbRows = NULL;
	eImageFileType imgType;
	image picture;

	unsigned char* gMyData = readTGA(kNameFile1, &imgType);
	//unsigned char* gMyData = readTGA(kNameFile1, &nbRows, &nbCols, &imgType);

	if (gMyData != NULL)
	{
		gMyTextInfo.nbCols = picture.nbrCols;
		gMyTextInfo.nbRows = picture.nbrRows;
		if (imgType == kTGA_COLOR)
			gMyTextInfo.bytesPerPixel = 3;
		else
			gMyTextInfo.bytesPerPixel = 1;

	}
	else
	{
		printf("Generation of image data failed.\n");
		return -1;
	}

	printf("Image read\n");


	int outOK = writeTGA(kNameFile2, picture);
	if (0)
	{
		printf("Image written.\n");
	}
	else
	{
		printf("Output of image data failed.\n");
		return -1;
	}

	return 0;
}
