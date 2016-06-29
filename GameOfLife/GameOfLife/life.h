#pragma once
#include "GlobalOptions.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "math_custom.h"
#include "life_generic.h"

void life_PopulateSimulation(int* matrix, int rows, int columns, double density);
void life_ConvertSimulation(int* screen, int screenWidth, int color, int* simulation, int rows, int columns, int cellSize);
int life_CountNeighborhood(int rows, int columns, int x, int y, int* matrix);
int* life_StepSimulation(int rows, int columns, int boundary, int* prevMatrix);