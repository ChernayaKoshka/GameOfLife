#pragma once

#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <ShlObj.h>
#include "math_custom.h"
#include "GlobalOptions.h"

#include "life_generic.h"

typedef struct tagWindowDetails
{
	int Width;
	int Height;
	void* BackBuffer;
	HDC DC;
	HWND Window;
	BITMAPINFO BitMapInfo;
}WindowDetails;