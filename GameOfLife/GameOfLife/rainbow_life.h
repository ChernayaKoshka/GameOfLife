#pragma once

#include "GlobalOptions.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include "math_custom.h"
#include "life_generic.h"

typedef struct tagNeighborhoodRbw
{
	int AverageColor;
	int Total;
}rbw_Neighborhood;

void rbw_PopulateSimulation(int* matrix, int rows, int columns, double density);
void rbw_ConvertSimulation(int* screen, int screenWidth, int color, int* simulation, int rows, int columns, int cellSize);
rbw_Neighborhood rbw_CountNeighborhood(int rows, int columns, int x, int y, int* matrix);
int* rbw_StepSimulation(int rows, int columns, int boundary, int* prevMatrix);
