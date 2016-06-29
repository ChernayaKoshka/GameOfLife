#include "rainbow_life.h"

void rbw_PopulateSimulation(int* matrix, int rows, int columns, double density)
{
	for (int y = 1; y < rows - 1; y++)
	{
		for (int x = 1; x < columns - 1; x++)
		{
			if (getRandomFloatInRange(0.0, 1.0) <= density)
			{
				matrix[(y*columns + x)] = GetRandRGB();
			}
			else
			{
				matrix[(y*columns + x)] = 0;
			}
		}
	}
}

void rbw_ConvertSimulation(int* screen, int screenWidth, int color, int* simulation, int rows, int columns, int cellSize)
{
	for (int y = 1; y < rows - 1; y++)
	{
		for (int x = 1; x < columns - 1; x++)
		{
			specialPlot(screen, screenWidth, (x - 1)*cellSize, (y - 1)*cellSize, cellSize, simulation[y*columns + x]);
		}
	}
}

/*
Counter Neighborhood

name: countNeighborhood

desc: This function will count all neighboring cells that are 'alive'

params: (1) int rows - The # of rows
(2) int columns - The # of columns
(3) int x - The column # of the cell
(4) int y - The row # of the cell
(5) char* matrix - The matrix provided will be searched for neighoring
cells.

returns: An integer representing the number of alive cells (including the main cell)
*/
rbw_Neighborhood rbw_CountNeighborhood(int rows, int columns, int x, int y, int* matrix)
{
	rbw_Neighborhood neighborhood = { 0 };

	int pivot = (y*columns + x);
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			int c = matrix[pivot + (i*columns) + j];
			if (c == 0) continue;

			neighborhood.AverageColor += c;
			neighborhood.Total++;
		}
	}

	if (neighborhood.Total != 0)
		neighborhood.AverageColor = neighborhood.AverageColor / neighborhood.Total;

	return neighborhood;
}

/*
Step Simulation

name: stepSimulation

desc: This function will "step" the simulation according to the game's rules

params: (1) int rows - The # of rows
(2) int columns - The # of columns
(3) int boundary - A boolean variable representing whether or not a
boundary should be applied.
(4) char* prevMatrix - The previous matrix that will be used to calculate the new matrix.

returns: A pointer to the stepped matrix.
*/
int* rbw_StepSimulation(int rows, int columns, int boundary, int* prevMatrix)
{
	int* steppedMatrix = calloc(rows*columns, sizeof(int));
	if (steppedMatrix == NULL)
		return NULL;

	for (int y = 1; y < rows - 1; y++)
	{
		for (int x = 1; x < columns - 1; x++)
		{
			rbw_Neighborhood live = rbw_CountNeighborhood(rows, columns, x, y, prevMatrix);
			int cell = *(prevMatrix + (y*columns + x));

			if (cell != 0) live.Total--;

			*(steppedMatrix + (y*columns + x)) = cell; //just making sure

			if (live.Total < 2)
			{
				steppedMatrix[y*columns + x] = 0; //manual indexing again
			}
			else if (live.Total > 3)
			{
				steppedMatrix[y*columns + x] = 0;
			}
			else if ((live.Total == 2 || live.Total == 3) && cell != 0)
			{
				steppedMatrix[y*columns + x] = cell;
			}
			else if (live.Total == 3 && cell == 0)
			{
				steppedMatrix[y*columns + x] = live.AverageColor;
			}
			else if (live.Total == 2)
			{
				//do nothing
			}
		}
	}
	free(prevMatrix); //free up previous matrix
	if (boundary)
		applyBorders(rows, columns, steppedMatrix); //apply border
	return steppedMatrix;
}