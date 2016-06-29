#pragma once
#include <Windows.h>

int* createSimulationMatrix(int rows, int columns, int boundary);
void applyBorders(int rows, int columns, int* matrix);
