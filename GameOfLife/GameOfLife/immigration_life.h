#pragma once
#include "GlobalOptions.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "math_custom.h"
#include "life_generic.h"

typedef struct tagNeighborhoodImm
{
	int Type1;
	int Type2;
	int Total;
}imm_Neighborhood;

void imm_PopulateSimulation(int* matrix, int rows, int columns, double density);
void imm_ConvertSimulation(int* screen, int screenWidth, int color, int* simulation, int rows, int columns, int cellSize);
imm_Neighborhood imm_CountNeighborhood(int rows, int columns, int x, int y, int* matrix);
int* imm_StepSimulation(int rows, int columns, int boundary, int* prevMatrix);