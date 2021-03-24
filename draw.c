#include "vcal.h"

void
clearscreen(void)
{
	draw(screen, screen->r, display->white, nil, ZP);
}

void
holdonbutton(int b, Mouse m)
{
	while(m.buttons & b){
		m = emouse();
		sleep(10);
	}
}

void
righttext(char *text, Point topright, int pad, Font *f)
{
	topright.x -= stringwidth(f, text) + pad;
	topright.y += pad;
	string(screen, topright, display->black, ZP, f, text);
}

void
centertext(char *text, Point center, Font *f)
{
	center = subpt(center, divpt(stringsize(f, text), 2));
	string(screen, center, display->black, ZP, f, text);
}

void
columntext(char **text, Point nw, int width, int numcols, Font *f)
{
	int i;

	nw.x += (width / numcols) / 2;
	for(i = 0; i < numcols; i++){
		centertext(text[i], nw, f);
		nw.x += (width / numcols) ;
	}
}

void
drawchev(Point tip, int w, int t)
{
	int h;
	Point p[6];

	if(w < 0)
		t = -t;

	h = (w - t);

	p[0] = tip;
	p[1] = Pt(tip.x + h, tip.y - h);
	p[2] = Pt(tip.x + w, tip.y - h);
	p[3] = Pt(tip.x + t, tip.y);
	p[4] = Pt(tip.x + w, tip.y + h);
	p[5] = Pt(tip.x + h, tip.y + h);

	fillpoly(screen, p, 6, 0, display->black, ZP);
}

Rectangle		/* returns bounding rectangle */
drawblocks(Rectangle region, int blocks, int cols, int skip, int start, int thick, int pad)
{
	Rectangle bounds;
	Point tile, nw, pos;
	int rows, tilewid, i;

	tile = Pt(skip, 0);
	rows = ((blocks + skip) / cols) + ((blocks + skip) % cols != 0);
	tilewid = min((Dy(region) + thick*(rows-1) - 2*pad) / rows,
					(Dx(region) + thick*(cols-1) - 2*pad) / cols);

	nw = addptint(region.min, pad);										/* sets top-left corner */
	nw.x += (Dx(region) + (cols-1)*thick - (2*pad + cols*tilewid)) / 2;	/* centers */

	for(i = 0; i < blocks; i++){
		pos = addpt(nw, mulpt(tile, tilewid - thick));		/* scales to tile's position and overlaps borders */

		border(screen, Rpt(pos, addptint(pos, tilewid)), thick, display->black, ZP);
		righttext(smprint("%d", i+start), Pt(pos.x + tilewid - thick, pos.y + thick), 3, defont);

		ptiterate(&tile, cols);
	}

	bounds.min = nw;
	bounds.max = addpt(nw, mulpt(Pt(cols, rows), tilewid - thick));
	bounds.max = addptint(bounds.max, thick);

	return bounds;
}

/*	TODO: color ideas i talked about - adding alternating greys to days, adding small tris/circles for events */