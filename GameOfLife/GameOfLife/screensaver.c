#include "screensaver.h"
#include "resource.h"

BOOL running = TRUE;

unsigned long int generation = 0;

wchar_t* titleFormat = L"Game of Life | Generation: %ld";
wchar_t title[256] = L"Game of Life | Generation: 0";

int ButtonMessage = 0;

int simulationMode = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, WPARAM lParam)
{
	LRESULT Result = 0;

	switch (uMsg)
	{
	case WM_KEYUP:
		return Result;
	case WM_ERASEBKGND:
		return TRUE;
	case WM_CLOSE:
		running = FALSE;
		return Result;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case RESTART_BTN:
			ButtonMessage = 1;
			break;
		case CYCLE_BTN:
			ButtonMessage = 2;
			break;
		}
		return Result;
	case WM_PAINT:
	default:
		Result = DefWindowProcW(hWnd, uMsg, wParam, lParam);
	}

	return Result;
}

WindowDetails* DefineWindow(HINSTANCE hInstance, int nShowCmd)
{
	WindowDetails* details = calloc(1, sizeof(WindowDetails));
	details->Height = WINDOW_HEIGHT;
	details->Width = WINDOW_WIDTH;

	int bufferSize = details->Height*details->Width * sizeof(int);
	details->BackBuffer = calloc(1, bufferSize); //4 = bytes to display RGB

	details->BitMapInfo.bmiHeader.biSize = sizeof(details->BitMapInfo.bmiHeader);
	details->BitMapInfo.bmiHeader.biWidth = details->Width;
	details->BitMapInfo.bmiHeader.biHeight = -details->Height;
	details->BitMapInfo.bmiHeader.biPlanes = 1;
	details->BitMapInfo.bmiHeader.biBitCount = 32;
	details->BitMapInfo.bmiHeader.biCompression = BI_RGB;

	//define window
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
#pragma warning(disable : 4028)
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	wc.lpszClassName = L"Game of Life";

	if (!RegisterClassEx(&wc))
		return NULL;

	RECT adjustedRect = { 0 };
	adjustedRect.bottom = WINDOW_HEIGHT;
	adjustedRect.right = WINDOW_WIDTH;
	AdjustWindowRect(&adjustedRect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE, FALSE);

	HWND hwndWindow = CreateWindowExW(
		0,
		L"Game of Life",
		&title,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		Difference(adjustedRect.left, adjustedRect.right),
		Difference(adjustedRect.bottom, adjustedRect.top),
		NULL,
		NULL,
		0,
		0);

	details->Window = hwndWindow;
	details->DC = GetDC(hwndWindow);
	ShowWindow(hwndWindow, nShowCmd);

	WNDCLASSEX childWC = { 0 };
	childWC.cbSize = sizeof(childWC);
#pragma warning(disable : 4028)
	childWC.lpfnWndProc = WndProc;
	childWC.hInstance = 0;
	childWC.hCursor = LoadCursor(NULL, IDC_ARROW);

	childWC.lpszClassName = L"Options";

	if (RegisterClassEx(&childWC))
	{
		RECT adjustedRect = { 0 };
		adjustedRect.bottom = 200;
		adjustedRect.right = 100;
		AdjustWindowRect(&adjustedRect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE, FALSE);

		HWND child = CreateWindowExW(
			0,
			L"Options",
			L"Options",
			WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			Difference(adjustedRect.left, adjustedRect.right),
			Difference(adjustedRect.bottom, adjustedRect.top),
			NULL,
			NULL,
			0,
			0);

		CreateWindowExW(0, L"BUTTON", L"Restart", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 0, 0, adjustedRect.right + 12, 20, child, (HMENU)RESTART_BTN, 0, NULL);
		CreateWindowExW(0, L"BUTTON", L"Cycle", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 0, 20, adjustedRect.right + 12, 20, child, (HMENU)CYCLE_BTN, 0, NULL);

		ShowWindow(child, nShowCmd);
	}



	return details;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WindowDetails* details = DefineWindow(hInstance, nShowCmd);
	MSG msg;

	int rows = details->Height / CELL_SIZE + 2;
	int columns = details->Width / CELL_SIZE + 2;

	int* sim = createSimulationMatrix(rows, columns, BORDERS);
	switch (simulationMode)
	{
	case 0:
		life_PopulateSimulation(sim, rows, columns, 0.35);
		break;
	case 1:
		imm_PopulateSimulation(sim, rows, columns, 0.35);
		break;
	case 2:
		rbw_PopulateSimulation(sim, rows, columns, 0.35);
		break;
	}
	clock_t prevTime = clock();

	while (running)
	{
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		switch (ButtonMessage)
		{
		case 1:
			free(sim);
			sim = createSimulationMatrix(rows, columns, BORDERS);
			switch (simulationMode)
			{
			case 0:
				life_PopulateSimulation(sim, rows, columns, 0.35);
				break;
			case 1:
				imm_PopulateSimulation(sim, rows, columns, 0.35);
				break;
			case 2:
				rbw_PopulateSimulation(sim, rows, columns, 0.35);
				break;
			}
			break;
		case 2:
			simulationMode++;
			if (simulationMode > 2)
				simulationMode = 0;

			free(sim);
			sim = createSimulationMatrix(rows, columns, BORDERS);
			switch (simulationMode)
			{
			case 0:
				life_PopulateSimulation(sim, rows, columns, 0.35);
				break;
			case 1:
				imm_PopulateSimulation(sim, rows, columns, 0.35);
				break;
			case 2:
				rbw_PopulateSimulation(sim, rows, columns, 0.35);
				break;
			}
			break;
		}
		ButtonMessage = 0;

		clock_t curTime = clock();
		if (((double)(curTime - prevTime) / CLOCKS_PER_SEC) >= STEPS_PER_SECOND)
		{
			prevTime = curTime;
			generation++;
			swprintf_s(title, 256, titleFormat, generation);
			SetWindowTextW(details->Window, title);

			int* newSim = NULL;
			switch (simulationMode)
			{
			case 0:
				newSim = life_StepSimulation(rows, columns, BORDERS, sim);
				life_ConvertSimulation(details->BackBuffer, details->Width, 0x002C4566, newSim, rows, columns, CELL_SIZE);
				break;
			case 1:
				newSim = imm_StepSimulation(rows, columns, BORDERS, sim);
				imm_ConvertSimulation(details->BackBuffer, details->Width, 0x002C4566, newSim, rows, columns, CELL_SIZE);
				break;
			case 2:
				newSim = rbw_StepSimulation(rows, columns, BORDERS, sim);
				rbw_ConvertSimulation(details->BackBuffer, details->Width, 0x002C4566, newSim, rows, columns, CELL_SIZE);
				break;
			}
			if (newSim == NULL)
				running = FALSE;
			sim = newSim;
		}

		StretchDIBits(details->DC,
			0, 0, details->Width, details->Height,
			0, 0, details->BitMapInfo.bmiHeader.biWidth, Abs(details->BitMapInfo.bmiHeader.biHeight),
			details->BackBuffer, &details->BitMapInfo,
			DIB_RGB_COLORS, SRCCOPY);
	}

	ReleaseDC(NULL, details->DC);
	free(details->BackBuffer);
	free(details);

	return EXIT_SUCCESS;
}
