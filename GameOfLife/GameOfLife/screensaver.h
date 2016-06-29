#pragma once

#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <ShlObj.h>
#include "math_custom.h"
#include "GlobalOptions.h"

#include "life.h"
#include "immigration_life.h"
#include "rainbow_life.h"

typedef struct tagWindowDetails
{
	int Width;
	int Height;
	void* BackBuffer;
	HDC DC;
	HWND Window;
	BITMAPINFO BitMapInfo;
}WindowDetails;