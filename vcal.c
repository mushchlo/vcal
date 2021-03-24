#include "vcal.h"

int  daysin[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };	/* non-leapyear */
char *mname[] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December", nil };
char *dname[] = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };

Threebutton button[2];
Yearscope viewing[4];
int mode = Month;
int opad = 10;

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
	Rectangle bounds, tileregion;
	Point p;
	int days, linepx, w;

	clearscreen();
	linepx = 3;

	tileregion = screen->r;
	tileregion.min.y += 4*defont->height;
	days = daysin[t.n] + (t.n == Feb && isleapyear(t.year));

	bounds = drawblocks(tileregion, days, 7, fstdaymonth(t), 1, linepx, opad);

	p = center(screen->r);
	p.y = screen->r.min.y + defont->height;
	w = maxstrwidth(mname, defont);

	centertext(mname[t.n], p, defont);										/* month name */
	button[0].region = centerrect(p, w + 40, defont->height + opad);
	border(screen, button[0].region, 2, display->black, ZP);				/* TODO: all of this should be condensed into some drawThreebutton() function */
	drawchev(Pt(p.x - (w - 22), p.y), 10, 3);
	drawchev(Pt(p.x + (w - 22), p.y), -10, 3);


	bounds.min.y -= defont->height;
	columntext(dname, bounds.min, Dx(bounds), 7, defont);	/* day names */
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
main(/*int argc, char **argv*/void)
{
	int menuresult, i;
	Mouse m;

	button[0].menu.item = mname;
	button[0].control   = &(viewing[Month].n);

	viewing[Month] = (Yearscope){ curyr(), curmo() };

	initdraw(0, 0, "calendar");
	eresized(0);
	einit(Emouse);

	for(;;){
		m = emouse();
		for(i = 0; i < 1; i++){
			if(ptinrect(m.xy, button[i].region)){
				if(m.buttons & 1)
					if(*(button[i].control) > Jan){
						--*(button[i].control);
						drawmonth(viewing[Month]);
						holdonbutton(1, m);
					}
				if(m.buttons & 2){
					menuresult = emenuhit(2, &m, &(button[i].menu));
					if(menuresult >= 0)
						viewing[Month] = (Yearscope){ 2021, menuresult };
					drawmonth(viewing[Month]);
					holdonbutton(2, m);
				}
				if(m.buttons & 4)
					if(*(button[i].control) < Dec){
						++*(button[i].control);
						drawmonth(viewing[Month]);
						holdonbutton(4, m);
					}
			}
		}
	}
}