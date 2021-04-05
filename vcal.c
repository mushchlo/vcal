#include "vcal.h"

char *mname[] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December", nil };
char *dname[] = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };

static Threebutton monthb;
static Threebutton yearb;
static Yearscope viewing[4];
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
	Point p₁, p₂;
	char *year;
	int days, linepx, w₁, w₂;

	clearscreen();
	linepx = 2;

	year = smprint("%d", viewing[mode].year);
	tileregion = screen->r;
	tileregion.min.y += 4*defont->height;
	days = daysin(t.n) + (t.n == Feb && isleapyear(t.year));

	bounds = drawblocks(tileregion, days, 7, fstdaymonth(t), 1, linepx, opad);

	p₁ = center(screen->r);
	p₁.y = screen->r.min.y + defont->height;
	w₁ = maxstrwidth(mname, defont) + 50;
	w₂ = stringwidth(defont, year) + 50;
	p₂.x = screen->r.max.x - (w₂/2 + opad);
	p₂.y = screen->r.min.y + defont->height;
	

	monthb.region = drawbutton(p₁, Pt(opad/3, opad), mname[t.n], w₁, linepx, defont);
	yearb.region = drawbutton(p₂, Pt(opad/3, opad), year, w₂, linepx, defont);

	bounds.min.y -= defont->height;
	columntext(dname, bounds.min, Dx(bounds), 7, defont);	/* day names */
}

void
drawyear(int y)
{
	
}

void
dec(Mouse *m, Menu *mn, Yearscope *ctl, int mode)
{
	switch(mode){
		case Year:
			ctl->year--;
			break;
		case Month:
			mondec(ctl);
			break;
	}
}

void
inc(Mouse *m, Menu *mn, Yearscope *ctl, int mode)
{
	switch(mode){
		case Year:
			ctl->year++;
			break;
		case Month:
			moninc(ctl);
			break;
	}
}

void
nsel(Mouse *m, Menu *mn, Yearscope *ctl, int mode)
{
	int sel;

	if((sel = emenuhit(m->buttons, m, mn)) >= 0)
		ctl->n = sel;
}

void
yearsel(Mouse *m, Menu *mn, Yearscope *ctl, int mode)
{
	char year[5];

	year[4] = 0;
	if(eenter("Jump to year:", year, 5, m) > 0)
		ctl->year = atoi(year);
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
main(/*int argc, char *argv[]*/void)
{
	Threebutton *button[Year+1];
	Mouse m;
	Menu monthmen;
	Event ev;
	int clicked, e, i;

	monthb = newbutton();
	monthb.action[Left]		= dec;
	monthb.action[Middle]	= nsel;
	monthb.action[Right]	= inc;

	yearb = newbutton();
	yearb.action[Left]		= dec;
	yearb.action[Middle]	= yearsel;
	yearb.action[Right]		= inc;

	monthmen.item = mname;

	button[Month] = &monthb;
	button[Year] = &yearb;

	viewing[Month] = (Yearscope){ curyr(), curmo() };

	initdraw(0, 0, "calendar");
	eresized(0);
	einit(Emouse);

	print("yep");
	for(;;){
//		e = event(&ev);
//		if(e == Ekeyboard && ev.kbdc == 27)
//			break;
		m = emouse();
		for(i = Month; i <= Year; i++)
			if(ptinrect(m.xy, button[i]->region) && m.buttons){
				clicked = m.buttons;
				button[i]->action[m.buttons](&m, &monthmen, &viewing[mode], i);	/* make this mode-agnostic */
				drawcalendar(mode, viewing[mode]);
				holdonbutton(clicked, m);
			}
	}
}