#include <stdlib.h>        
#include <stdio.h>
#include <string.h>
//
#include "fileIO_TGA.h"

void swapRGB(unsigned char *theData, int nbRows, int nbCols);

//----------------------------------------------------------------------
//	Utility function for memory swapping
//	Used because TGA stores the RGB data in reverse order (BGR)
//----------------------------------------------------------------------
void swapRGB(unsigned char *theData, int nbRows, int nbCols)
{
	int			imgSize;
	unsigned char tmp;

	imgSize = nbRows*nbCols;

	for (int k = 0; k < imgSize; k++)
	{
		tmp = theData[k * 3 + 2];
		theData[k * 3 + 2] = theData[k * 3];
		theData[k * 3] = tmp;
	}
}



// ---------------------------------------------------------------------
//	Function : readTGA 
//	Description :
//	
//	This function reads an image of type TGA (8 or 24 bits, uncompressed
//	
//	Parameters:
//
//	*fileName - Pointer to a string that contains the name of the file
//	*nbCols, *nbRows - Dimensions XxY of the image
//	
//	Return value: pointer to the pixel data
//----------------------------------------------------------------------

unsigned char *readTGA(char *fileName, eImageFileType *theType)
//unsigned char *readTGA(char *fileName, int *nbCols, int *nbRows, eImageFileType *theType)
{
	image* picture = (image*)calloc(1, sizeof(image));
	unsigned char *ptr;
	//int		numberOfPixels;
	//char	head[18] ;
	FILE	*tga_in;

	/* --- open TARGA input file ---------------------------------- */
	tga_in = fopen(fileName, "rb");

	if (tga_in == NULL)
	{
		printf("Cannot open pixel file %s \n", fileName);
		return NULL;
	}

	/* --- Read the header (TARGA file) --- */
	fread(picture->head, sizeof(char), 18, tga_in);
	/* Get the size of the image */
	picture->nbrCols = (int)((unsigned int)picture->head[12] & 0xFF | (unsigned int)picture->head[13] * 256);
	picture->nbrRows = (int)((unsigned int)picture->head[14] & 0xFF | (unsigned int)picture->head[15] * 256);


	if ((picture->head[2] == 2) && (picture->head[16] == 24))
		*theType = kTGA_COLOR;
	else if ((picture->head[2] == 3) && (picture->head[16] == 8))
		*theType = kTGA_GRAY;
	else
	{
		printf("Unsuported TGA image: ");
		printf("Its type is %d and it has %d bits per pixel.\n", picture->head[2], picture->head[16]);
		printf("The image must be uncompressed while having 8 or 24 bits per pixel.\n");
		fclose(tga_in);
		//std::exit(kWrongFileType);
		return NULL;
	}

	picture->numberOfPixels = picture->nbrCols * picture->nbrRows;
	/* Create the buffer for image */

	if (*theType == kTGA_COLOR)
		picture->theData = (unsigned char*)malloc(picture->numberOfPixels * 3);
	else
		picture->theData = (unsigned char*)malloc(picture->numberOfPixels);

	if (picture->theData == NULL)
	{
		printf("Unable to allocate memory\n");
		fclose(tga_in);
		//std::exit(kMemAllocError);
		return NULL;
	}

	/* Check if the image is vertically mirrored */
	if (*theType == kTGA_COLOR)
	{
		// preallocate an array to store one row.
		unsigned char* oneRow = (unsigned char*)malloc((picture->nbrCols) * 3);

		// image is written upside down
		if (picture->head[17] & 0x20)
		{
			//        --------------------------  end of the array
			ptr = picture->theData + picture->numberOfPixels * 3 - ((picture->nbrCols) * 3);
			//                                     --------------  size of one row
			for (int i = 0; i < picture->nbrRows; i++)
			{
				//          ------------------------ one row worth of data
				fread(ptr, 3 * sizeof(char), picture->nbrCols, tga_in);

				// copy oneRow into the data array
				memcpy(ptr, oneRow, (picture->nbrCols) * 3);

				ptr -= (picture->nbrCols) * 3;
			}

			// delete oneRow
			//			free(oneRow);
			free((void*)oneRow);
		}
		else
			//  destination  size of data chunk  number of chunk   source
			fread(picture->theData, 3 * sizeof(char), picture->numberOfPixels, tga_in);

		//  tga files store color information in the order B-G-R
		//  we need to swap the Red and Blue components
		swapRGB(picture->theData, picture->nbrRows, picture->nbrCols);
	}
	else
	{
		if (picture->head[17] & 0x20)
		{
			ptr = picture->theData + picture->numberOfPixels - picture->nbrCols;
			for (int i = 0; i < picture->nbrRows; i++)
			{
				fread(ptr, sizeof(char), picture->nbrCols, tga_in);
				ptr -= picture->nbrCols;
			}
		}
		else
			fread(picture->theData, sizeof(char), picture->numberOfPixels, tga_in);
	}

	fclose(tga_in);
	return(picture->theData);
}
/*
 frees the info stored in the picture location
*/
int freeImage(image* picture) {

free(picture->head);
free(picture->nbrCols);
free(picture->nbrRows);
free(picture->numberOfPixels);
free(picture->theData);

return 0;


//---------------------------------------------------------------------*
//	Function : writeTGA 
//	Description :
//	
//	 This function write out an image of type TGA (24-bit color)
//	
//	 Parameters:
//	
//	  *fileName - Pointer to a string that contains the name of the file
//	  nbCols, nbRows - Dimensions XxY of the image
//	  *data - pointer to the array containing the pixels. This array
//	          is of type char {r, g, b, r, g, b, ... }
//
//	Return value: Error code (0 = no error)
//----------------------------------------------------------------------*/ 
//int writeTGA(char *fileName, unsigned char *theData, int nbRows, int nbCols)
int writeTGA(char *fileName, image picture)
{
	int		i, j;
	long	offset;
	int		swapflag = 1;
	//char	head[18] ;
	FILE	*tga_out;

	/* --- open TARGA output file ---------------------------------- */

	tga_out = fopen(fileName, "wb");

	if (!tga_out)
	{
		printf("Cannot create pixel file %s \n", fileName);
		return(kCannotOpenWrite);
	}

	// --- create the header (TARGA file) ---
	picture.head[0] = 0;		  					// ID field length.
	picture.head[1] = 0;		  					// Color map type.
	picture.head[2] = 2;		  					// Image type: true color, uncompressed.
	picture.head[3] = picture.head[4] = 0;  				// First color map entry.
	picture.head[5] = picture.head[6] = 0;  				// Color map lenght.
	picture.head[7] = 0;		  					// Color map entry size.
	picture.head[8] = picture.head[9] = 0;  				// Image X origin.
	picture.head[10] = picture.head[11] = 0; 				// Image Y origin.
	picture.head[13] = (char)(picture.nbrCols >> 8);		// Image width.
	picture.head[12] = (char)(picture.nbrCols & 0x0FF);
	picture.head[15] = (char)(picture.nbrRows >> 8);		// Image height.
	picture.head[14] = (char)(picture.nbrRows & 0x0FF);
	picture.head[16] = 24;		 					// Bits per pixel.
	picture.head[17] = 0;		  					// Image descriptor bits ;
	fwrite(picture.head, sizeof(char), 18, tga_out);

	for (i = 0; i < picture.nbrRows; i++)
	{
		offset = i * 3 * picture.nbrCols;
		for (j = 0; j < picture.nbrCols; j++)
		{
			fwrite(&picture.theData[offset + 2], sizeof(char), 1, tga_out);
			fwrite(&picture.theData[offset + 1], sizeof(char), 1, tga_out);
			fwrite(&picture.theData[offset], sizeof(char), 1, tga_out);
			offset += 3;
		}
	}

	fclose(tga_out);

	return(kNoIOerror);
}
