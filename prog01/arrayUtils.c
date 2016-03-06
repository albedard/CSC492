/*
* arrayUtils.c
*
*  Created on: Feb 28, 2016
*      Author: Amanda
*/

//The include statement for the standard library
#include <stdbool.h>
#include <stdlib.h>

//Includes the header with the function names
#include "arrayUtils.h"



/*
* These functions dynamically allocates a 1D array of ints and return a pointer to that array.
* It takes the row max and min to determine the values that need to be entered.  The parameters
* are the values for the max and min rows
*/
unsigned char* ucVector(int rowMin, int rowMax) {

	//Malloc returns null if unsuccessful so pointer will be empty
	unsigned char* up1 = (unsigned char*)malloc((rowMax - rowMin + 1)*sizeof(unsigned char));

	return up1;
}
int* iVector(int rowMin, int rowMax) {

	//Allocates array of ints for rowMin - rowMax
	// array of spots 6 to 12 instead of 0 to 6
	int* ip1 = (int*)malloc((rowMax - rowMin + 1)*sizeof(int));

	//returns a pointer to the array
	return ip1;
}
/*
* These functions create a 2D array.  It first creates an array for the 'rows' and then
* uses pointers to create the columns as pointers to arrays. The parameters
* are the min and max for rows and columns, and it returns a pointer to a pointer to the
* 2D array.
*/
unsigned char** ucMatrix(int rowMin, int rowMax, int colMin, int colMax) {

	unsigned char** uarr2d = (unsigned char**)malloc((rowMax - rowMin + 1)*sizeof(unsigned char*));

	for (int i = rowMin; i <= rowMax; i++) {
		uarr2d[i] = ucVector(colMin, colMax);
	}

	return uarr2d;

}
int** iMatrix(int rowMin, int rowMax, int colMin, int colMax) {


	//Creates the initial array
	int** arr2d = (int**)malloc((rowMax - rowMin + 1)*sizeof(int*));

	//arr2d -= rowMin;

	for (int i = rowMin; i <= rowMax; i++) {

		//for each row, adds columns in an array
		arr2d[i] = iVector(colMin, colMax);

	}


	//returns the 2d array
	return arr2d;
}
/*
* These functions  free the 1D array. It takes the array to be freed and the
* row minimum as parameters, and returns 0 if the array has successfully been freed
*/
//Frees the unsigned char vector v
int free_ucVector(unsigned char* v, int rowMin) {

	//Frees the whole array
	free(v += rowMin);

	//Returns 0 when successful
	return 0;

}

//Frees the int vector v
int free_iVector(int* v, int rowMin) {


	//Frees the whole array
	free(v += rowMin);

	//When finished
	return 0;

}

/*
* This function is to free a 2D array.  It takes the row max, row min, and column min as
* parameters, and returns 0 if it was successfully freed.
*/
int free_iMatrix(int** m, int rowMin, int rowMax, int colMin) {

	//loops and deletes all the array elements

	for (int i = rowMin; i <= rowMax; i++) {

		//Frees the array of pointers stored in the pointer value
		free_iVector(m[i], colMin);

	}

	//Deletes the final array
	free(m + rowMin);

	//returns 0 when complete
	return 0;
}

//Scaffolding matrix.
int** iMatrixFromArray1D(int* a, int rowMin, int rowMax, int colMin, int colMax) {

	//'maps' the 2d array on top of the 1d array
	int** a2D = (int**)calloc((rowMax - rowMin + 1), sizeof(int*));

	a2D[0] = a;

	for (int i = 0; i < (rowMax - rowMin + 1); i++)
		a2D[i] = a2D[i - 1] + (colMax - colMin + 1);

	//returns the mapped array
	return a2D;
}

//Scaffolding unsigned char matrix
unsigned char** ucMatrixFromArray1D(unsigned char* a, int rowMin, int rowMax, int colMin, int colMax) {

	//'maps' the 2d array on top of the 1d array
	unsigned char** ua2D = (unsigned char**)calloc((rowMax - rowMin + 1), sizeof(unsigned char*));

	ua2D[0] = a;

	for (int i = 0; i < (rowMax - rowMin + 1); i++)
		ua2D[i] = ua2D[i - 1] + (colMax - colMin + 1);

	//returns the mapped array
	return ua2D;
}

//frees the matrix
int free_iScaffoldingMatrix(int** m, int rowMin) {

	//frees the structure on the 1D raster
	free(m + rowMin);

	//Returns 0 when done
	return 0;

}

int free_ucScaffoldingMatrix(unsigned char** m, int rowMin) {

	//frees the structure on the 1D raster
	free(m + rowMin);

	//Returns 0 when done
	return 0;

}
