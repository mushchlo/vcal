#include "vcal.h"

int
min(int a, int b)
{
	if(a < b)
		return a;
	return b;
}

int
modulo(int a, int n)
{
	int r;

	r = a % n;
	return (r < 0) ? r + n : r;
}

Point
addptint(Point p, int x)
{
	return addpt(p, (Point){x,x});
}