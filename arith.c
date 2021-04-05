#include "vcal.h"

int
min(int a, int b)
{
	return (a < b) ? a : b;
}

int
max(int a, int b)
{
	return (a > b) ? a : b;
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

void
ptiterate(Point *p, int wrap)	/* basically a modulus iterator for points, "wraps" along x axis */
{
	if(p->x >= wrap-1)
		p->x = 0, p->y++;
	else
		p->x++;
}

Point
center(Rectangle r)
{
	return Pt(r.min.x + Dx(r)/2, r.min.y + Dy(r)/2);
}

Rectangle
centerrect(Point center, int w, int h)
{
	Rectangle r;

	r = Rpt(center, center);

	r.min.x -= w/2;
	r.min.y -= h/2;
	r.max.x += w/2;
	r.max.y += h/2;

	return r;
}

int
maxstrwidth(char **str, Font *f)	/* returns the string of largest graphical width, str is nil-terminated */
{
	int w;

	for(w = 0; *str != nil; str++)
		w = max(w, stringwidth(f, *str));

	return w;
}