#include "life.h"

void life_PopulateSimulation(int* matrix, int rows, int columns, double density)
{
	for (int y = 1; y < rows - 1; y++)
	{
		for (int x = 1; x < columns - 1; x++)
		{
			if (getRandomFloatInRange(0.0, 1.0) < density)
				matrix[y*columns + x] = 1;
			else
				matrix[y*columns + x] = 0;
		}
	}
}

void life_ConvertSimulation(int* screen, int screenWidth, int color, int* simulation, int rows, int columns, int cellSize)
{
	for (int y = 1; y < rows - 1; y++)
	{
		for (int x = 1; x < columns - 1; x++)
		{
			if (simulation[y*columns + x] == 1)
				specialPlot(screen, screenWidth, (x - 1)*cellSize, (y - 1)*cellSize, cellSize, color);
			else
				specialPlot(screen, screenWidth, (x - 1)*cellSize, (y - 1)*cellSize, cellSize, 0x0);
		}
	}
}

int life_CountNeighborhood(int rows, int columns, int x, int y, int* matrix)
{
	int count = 0;

	int pivot = (y*columns + x);
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			char c = matrix[pivot + (i*columns) + j];

			if (c == 1) count++;
		}
	}
	return count;
}

int* life_StepSimulation(int rows, int columns, int boundary, int* prevMatrix)
{
	int* steppedMatrix = malloc(rows*columns*sizeof(int));

	if (steppedMatrix == NULL)
		return NULL;

	for (int y = 1; y < rows - 1; y++)
	{
		for (int x = 1; x < columns - 1; x++)
		{
			int live = life_CountNeighborhood(rows, columns, x, y, prevMatrix);
			char cell = *(prevMatrix + (y*columns + x));
			if (cell == 1) live--;  //avoid counting self

			*(steppedMatrix + (y*columns + x)) = cell; //just making sure

			if (live < 2)
			{
				steppedMatrix[y*columns + x] = 0; //manual indexing again
			}
			else if (live > 3)
			{
				steppedMatrix[y*columns + x] = 0;
			}
			else if ((live == 2 || live == 3) && cell == 1)
			{
				steppedMatrix[y*columns + x] = 1;
			}
			else if (live == 3 && cell == 0)
			{
				steppedMatrix[y*columns + x] = 1;
			}
			else if (live == 2)
			{
				//do nothing
			}
		}
	}
	free(prevMatrix); //free up previous matrix
	if (boundary) applyBorders(rows, columns, steppedMatrix); //apply border
	return steppedMatrix;
}