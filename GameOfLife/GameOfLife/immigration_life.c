#include "immigration_life.h"

void imm_PopulateSimulation(int* matrix, int rows, int columns, double density)
{
	for (int y = 1; y < rows - 1; y++)
	{
		for (int x = 1; x < columns - 1; x++)
		{
			if (getRandomFloatInRange(0.0, 1.0) <= density)
			{
				if (rand() % 2 == 1)
					matrix[(y*columns + x)] = 1;
				else
					matrix[(y*columns + x)] = 2;
			}
			else
			{
				matrix[(y*columns + x)] = 0;
			}
		}
	}
}

void imm_ConvertSimulation(int* screen, int screenWidth, int color, int* simulation, int rows, int columns, int cellSize)
{
	for (int y = 1; y < rows - 1; y++)
	{
		for (int x = 1; x < columns - 1; x++)
		{
			if (simulation[y*columns + x] == 1)
				specialPlot(screen, screenWidth, (x - 1)*cellSize, (y - 1)*cellSize, cellSize, color);
			else if (simulation[y*columns + x] == 2)
				specialPlot(screen, screenWidth, (x - 1)*cellSize, (y - 1)*cellSize, cellSize, color*1.5);
			else
				specialPlot(screen, screenWidth, (x - 1)*cellSize, (y - 1)*cellSize, cellSize, 0x0);
		}
	}
}

imm_Neighborhood imm_CountNeighborhood(int rows, int columns, int x, int y, int* matrix)
{
	imm_Neighborhood neighborhood = { 0 };

	int pivot = (y*columns + x);
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			char c = matrix[pivot + (i*columns) + j];

			if (c == 1)
			{
				neighborhood.Type1++;
			}
			else if (c == 2)
			{
				neighborhood.Type2++;
			}
		}
	}
	neighborhood.Total = neighborhood.Type1 + neighborhood.Type2;
	return neighborhood;
}

int* imm_StepSimulation(int rows, int columns, int boundary, int* prevMatrix)
{
	int* steppedMatrix = malloc(rows*columns*sizeof(int));
	if (steppedMatrix == NULL)
		return NULL;

	for (int y = 1; y < rows - 1; y++)
	{
		for (int x = 1; x < columns - 1; x++)
		{
			imm_Neighborhood live = imm_CountNeighborhood(rows, columns, x, y, prevMatrix);
			char cell = *(prevMatrix + (y*columns + x));

			if (cell == 1)
			{
				live.Type1--;
				live.Total--;
			}
			else if (cell == 2)
			{
				live.Type2--;
				live.Total--;
			}

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
				if(cell == 1)
					steppedMatrix[y*columns + x] = 1;
				else if(cell == 2)
					steppedMatrix[y*columns + x] = 2;
			}
			else if (live.Total == 3 && cell == 0)
			{
				if(live.Type1 > live.Type2)
					steppedMatrix[y*columns + x] = 1;
				else
					steppedMatrix[y*columns + x] = 2;
			}
			else if (live.Total == 2)
			{
				//do nothing
			}
		}
	}
	free(prevMatrix); //free up previous matrix
	if (boundary) applyBorders(rows, columns, steppedMatrix); //apply border
	return steppedMatrix;
}