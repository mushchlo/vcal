#include <u.h>
#include <libc.h>
#include <draw.h>
#include <event.h>
#define SWAP(x, y, T) do{ T SWAPTMP = x; x = y; y = SWAPTMP; } while(0)
#define defont display->defaultfont

enum { Day, Week, Month, Year };
enum { Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec };

int daysin[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };	/* this is for non-leap year. */
char *monthnames[] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December", nil };
char *daynames[]   = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };

struct Date {
	int year;	/* CE */
	int day;	/* 1 - 365/366 */
};
typedef struct Date Date;

struct Yearscope {
	int year;
	int n;
};
typedef struct Yearscope Yearscope;

Menu months;

Yearscope viewing[4];
int mode = Month;
int opad = 10;


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

int
isleapyear(int y)
{
	if(y%4 == 0 && y%100 != 0
		|| y%400 == 0)
		return 1;
	return 0;
}

int
before(Date before, Date after)
{
	if(before.year > after.year)
		return 0;
	if(before.year < after.year)
		return 1;
	if(before.day > after.day)
		return 0;
	return 1;
}

int
leapdaysbetween(Date d₁, Date d₂)	/* includes both d₁ and d₂ in the count */
{
	int i, ld, rev;

	ld = 0;
	rev = 1;
	if(!before(d₁, d₂)){
		SWAP(d₁, d₂, Date);
		rev = -1;
	}

	for(i = d₁.year; i < d₂.year; i++)
		if(isleapyear(i))
			ld++; 

	return ld*rev;
}

int
daysbetween(Date d₁, Date d₂)
{
	int Δy, Δd;

	Δy = d₂.year - d₁.year;
	Δd = d₂.day - d₁.day;

	return (365*Δy) + Δd + leapdaysbetween(d₁, d₂);	/* returns negative values
														if d₁ is greater than d₂ */
}

int
dayofweek(Date d)	/* Monday is 0, Sunday 6 */
{
	Date refdate;

	refdate = (Date){ 1900, 1 };	/* Jan 1st, 1900 was a Monday */
	return modulo(daysbetween(refdate, d), 7);
}

Date
todate(int y, int m, int d)
{
	int i;

	for(i = Jan; i < m; i++)
		d += daysin[i] + (m == Feb && isleapyear(y));

	return (Date){y, d};
}

int
fstdaymonth(Yearscope t)
{
	return dayofweek(todate(t.year, t.n, 1));
}

int
fsdayweek(Yearscope t)	/* ordinal weeks are as spoken, so 1st week is week 1 */
{
	return dayofweek((Date){t.year, 1+7*((t.n)-1)});
}

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
drawday(Yearscope d)
{
	
}

void
drawweek(Yearscope w)
{
	
}

void
drawmonth(Yearscope m)
{
	Rectangle region;
	int days, wid, linepx;

	clearscreen();

	linepx = 2;
	days = daysin[m.n] + (m.n == Feb && isleapyear(m.year));
	region = screen->r;
	region.min.y += 4*defont->height;

	wid = drawblocks(region, days, 7, fstdaymonth(m), 1, linepx, opad);
	centertext(monthnames[m.n],
				Pt(screen->r.min.x + 7*(wid - linepx)/2, screen->r.min.y + defont->height),		/* month name */
				defont);
	columntext(daynames,
				Pt(screen->r.min.x + opad, screen->r.min.y + 3*defont->height + opad/2),		/* day names */
				7*(wid - linepx), 7, defont);
}

void
drawyear(Yearscope y)
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
		drawyear(curr);
		drawmonth(curr);
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