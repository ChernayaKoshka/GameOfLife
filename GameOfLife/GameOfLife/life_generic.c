#include "life_generic.h"

/*
Apply Borders

name: applyBorders

desc: This function will begin apply cell boarders, copying the visible row into
an invisible one to be used for calculations. These rows will not be treated
as though they are alive themselves.

params: (1) int rows - The # of rows
(2) int columns - The # of columns
(3 - VOLATILE) char* matrix - the matrix to apply borders to.

returns: Nothing, but will modify the provided array

author note: I'm really sorry, I didn't see your heap-matrix thing until it was way too late.
so everything is indexed doing the math manually.
*/
void applyBorders(int rows, int columns, int* matrix)
{
	//top/bottom
	for (int x = 1; x < columns - 1; x++)
	{
		matrix[x] = matrix[x + columns*(rows - 2)];
		matrix[x + columns*(rows - 1)] = matrix[x + columns];
	}

	//sides
	for (int y = 1; y < rows - 1; y++)
	{
		matrix[y*columns] = matrix[y*columns + columns - 2];
		matrix[y*columns + columns - 1] = matrix[y*columns + 1];
	}

	//top left corner
	matrix[0] = matrix[columns*(rows - 2) + columns - 2];
	//top right corner
	matrix[columns - 1] = matrix[columns*(rows - 2) + 1];
	//bottom left corner
	matrix[columns*(rows - 1)] = matrix[columns * 2 - 2];
	//bottom right corner
	matrix[columns*(rows - 1) + columns - 1] = matrix[columns + 1];
}

int* createSimulationMatrix(int rows, int columns, int boundary)
{
	int* matrix = malloc(rows*columns*sizeof(int));

	if (matrix == NULL)
		return NULL;

	for (int y = 0; y < rows; y++)
		for (int x = 0; x < columns; x++)
			matrix[y*columns + x] = 0;

	if (boundary) 
		applyBorders(rows, columns, matrix);

	return matrix;
}