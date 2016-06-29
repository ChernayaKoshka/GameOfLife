#include "math_custom.h"

int Greatest(int num, int num2)
{
	if (num > num2) return num;
	return num2;
}

int Least(int num, int num2)
{
	if (num < num2) return num;
	return num2;
}

int Difference(int num, int num2)
{
	return Greatest(num, num2) - Least(num, num2);
}

int Abs(int val)
{
	if (val < 0) return -val;
	return val;
}

/*
Get Random Float In Range

name: getRandomFloatInRange

desc: This function will generate a random floating point number between
the min and max

params: (1) int min - The minimum size of the floating point number
(2) int max - The maximum size of the floating point number

returns: An integer representing the number of tries allowed to the user
*/
double getRandomFloatInRange(double min, double max)
{
	return (((double)rand() / RAND_MAX)*(max - min)) + min;
}

int GetColor(double* angle)
{
	*angle += 0.05;
	if (*angle > 2 * M_PI)
		*angle = -(2 * M_PI);
	return RGB(sin(*angle + 0) * 127 + 128, sin(*angle + 2) * 127 + 128, sin(*angle + 4) * 127 + 128);
}

int GetRandRGB()
{
	int color = 0;
	while (!color)
		color = RGB(rand() % 256, rand() % 256, rand() % 256);
	return color;
}

//"special plot" = square plot
void specialPlot(int* screen, int screenWidth, int x, int y, int size, int color)
{
	for (int i = 0; i < size; i++)
	{
		for (int z = 0; z < size; z++)
		{
			screen[((y + z)*screenWidth) + x + i] = color;
		}
	}
}

Vector getVector(POINT p1, POINT p2)
{
	Vector toReturn = { 0 };
	toReturn.i = p2.x - p1.x;
	toReturn.j = p2.y - p1.y;
	return toReturn;
}

int dotProduct(Vector p1, Vector p2)
{
	return ((p1.i*p2.i) + (p1.j*p2.j));
}

POINT* convertRectToPoints(RECT rect)
{
	POINT* points = malloc(4 * sizeof(POINT));

	int height = Difference(rect.top, rect.bottom);
	int width = Difference(rect.left, rect.right);

	points[2].x = rect.left;
	points[2].y = height;

	points[3].x = width;
	points[3].y = height;

	points[0].x = rect.left;
	points[0].y = Least(rect.top, rect.bottom);

	points[1].x = width;
	points[1].y = Least(rect.top, rect.bottom);

	return points;
}

//uses vectors for fancy stuff :)
	//formula: (0<AM[dot]AB<AB[dot]AB)/\(0<AM[dot]AD<AD[dot]AD) where M is a point an A,B,D are top left, top right, and bottom right respectively. [dot] represents the dot as in dot product.
int rectContainsPoint(POINT rect[4], POINT point)
{
	Vector AM = getVector(rect[0], point);
	Vector AB = getVector(rect[0], rect[1]);
	Vector BC = getVector(rect[1], rect[2]);
	Vector BM = getVector(rect[1], point);

	int dotAMAB = dotProduct(AM, AB);
	int dotABAB = dotProduct(AB, AB);
	int dotBCBM = dotProduct(BC, BM);
	int dotBCBC = dotProduct(BC, BC);

	if (((0 <= dotAMAB) && (dotAMAB <= dotABAB)) && ((0 <= dotBCBM) && (dotBCBM <= dotBCBC)))
		return 1;
	else
		return 0;
}