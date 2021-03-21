#include "vcal.h"

int daysin[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };	/* this is for non-leap year. */
char *monthnames[] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December", nil };
char *daynames[]   = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };

Yearscope viewing[4];
int mode = Month;
int opad = 10;
Menu months;


void
clearscreen(void)
{
	draw(screen, screen->r, display->white, nil, ZP);
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
columntext(char **text, Point left, int width, int numcols, Font *f)
{
	int i;

	left.x += (width / numcols) / 2;
	for(i = 0; i < numcols; i++){
		centertext(text[i], left, f);
		left.x += width / numcols;
	}
}

int		/* returns width of block */
drawblocks(Rectangle region, int blocks, int cols, int skip, int start, int barwidth, int pad)
{
	Point tile, pos;
	int rows, tilewid, i;

	tile = Pt(skip, 0);
	rows = ((blocks / cols) + (blocks%cols != 0));

	tilewid = min((Dy(region) + barwidth*(rows-1) - 2*pad) / rows,
					(Dx(region) + barwidth*(cols-1) - 2*pad) / cols);

	for(i = 0; i < blocks; i++){
		if(i != 0 && tile.x % cols == 0){
			tile.x = 0;
			tile.y++;
		}

		pos = addpt(mulpt(tile, tilewid), addptint(region.min, pad));
		pos = subpt(pos, mulpt(tile, barwidth));

		border(screen, Rpt(pos, addptint(pos, tilewid)), barwidth, display->black, ZP);
		righttext(smprint("%d", i+start), Pt(pos.x + tilewid - barwidth, pos.y + barwidth), 3, defont);
		tile.x++;
	}

	return tilewid;
}

void
drawday(Yearscope t)
{
	
}

void
drawweek(Yearscope t)
{
	
}

void
drawmonth(Yearscope t)
{
	Rectangle region;
	int days, wid, linepx;

	clearscreen();

	linepx = 2;
	days = daysin[t.n] + (t.n == Feb && isleapyear(t.year));
	region = screen->r;
	region.min.y += 4*defont->height;

	wid = drawblocks(region, days, 7, fstdaymonth(t), 1, linepx, opad);
	centertext(monthnames[t.n],
				Pt(screen->r.min.x + 7*(wid - linepx)/2, screen->r.min.y + defont->height),		/* month name */
				defont);
	columntext(daynames,
				Pt(screen->r.min.x + opad, screen->r.min.y + 3*defont->height + opad/2),		/* day names */
				7*(wid - linepx), 7, defont);
}

void
drawyear(int y)
{
	
}

void
drawcalendar(int scope, Yearscope curr)
{
	switch(scope){
	case Day:
		drawday(curr);
		break;
	case Week:
		drawweek(curr);
		break;
	case Month:
		drawmonth(curr);
		break;
	case Year:
		drawyear(curr.year);
		break;
	}
}

void
eresized(int new)
{
	if(new && getwindow(display, Refnone) < 0)
		sysfatal("can't reattach to window");
	drawcalendar(mode, viewing[mode]);
}

void
testlpyr(void)
{
	int tests[] = { 2000, 1900, 100, 2020, 2016, 2015 };
	int ans[]   = { 1, 0, 0, 1, 1, 0 };
	int i;

	for(i = 0; i < 5; i++)
		if(isleapyear(tests[i]) != ans[i])
			print("incorrect leap year answer for year %d\n", tests[i]);
}

void
main(int argc, char **argv)
{
	int menuresult;
	Mouse m;

	testlpyr();

	viewing[Month] = (Yearscope){ 2021, Jan };
	months.item = monthnames;

	initdraw(0, 0, "calendar");
	eresized(0);
	einit(Emouse);
	for(;;){
		m = emouse();
		if(m.buttons & 4)
			break;
		if(m.buttons & 2){
			menuresult = emenuhit(2, &m, &months);
			if(menuresult > 0){
				viewing[Month] = (Yearscope){ 2021, menuresult };
				drawmonth(viewing[Month]);
			}
		}
	}
//	print("%d\n", firstdayofmonth(1899, Dec));
}