#pragma once
#include <Windows.h>

#include "life.h"
#include "immigration_life.h"
#include "rainbow_life.h"

int* createSimulationMatrix(int rows, int columns, int boundary);
void applyBorders(int rows, int columns, int* matrix);
void populateSimulation(int mode, int* matrix, int rows, int columns, double density);
int* stepSimulation(int mode, int* screen, int screenWidth, int* prevMatrix, int rows, int columns, int boundary, int cellSize);
