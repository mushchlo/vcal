#include "vcal.h"

char *mname[] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December", nil };
char *dname[] = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun", nil };

static Threebutton monb;
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
	p₁.y = screen->r.min.y + defont->height/2 + 4*opad/3;
	w₁ = maxstrwidth(mname, defont) + 50;
	w₂ = stringwidth(defont, year) + 50;
	p₂.x = screen->r.min.x + (w₂/2 + opad);
	p₂.y = screen->r.min.y + defont->height/2 + 4*opad/3;

	monb.region = drawbutton(p₁, Pt(opad/3, opad), mname[t.n], w₁, linepx, defont);
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
			ndec(ctl, Jan, Dec);
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
			ninc(ctl, Jan, Dec);
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

	year[0] = 0;
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
main(int argc, char *argv[])
{
	Threebutton *button[Year+1];
	Menu monthmen;
	Event ev;
	Mouse m;
	int clicked, etype, i;

	monb = newbutton();
	monb.action[Left]	= dec;
	monb.action[Mid]	= nsel;
	monb.action[Right]	= inc;

	yearb = newbutton();
	yearb.action[Left]	= dec;
	yearb.action[Mid]	= yearsel;
	yearb.action[Right]	= inc;

	monthmen.item = mname;

	button[Month] = &monb;
	button[Year] = &yearb;

	viewing[Month] = (Yearscope){ curyr(), curmo() };

	USED(argv, argc);
	initdraw(0, 0, "calendar");
	einit(Ekeyboard|Emouse);
	eresized(0);

	for(;;){
		etype = event(&ev);
		if(etype == Emouse){
			m = ev.mouse;
			for(i = Month; i <= Year; i++)
				if(ptinrect(m.xy, button[i]->region) && m.buttons){
					clicked = m.buttons;
					button[i]->action[m.buttons](&m, &monthmen, &viewing[mode], i);	/* make this mode-agnostic */
					drawcalendar(mode, viewing[mode]);
					holdonbutton(clicked, m);
				}
		}
		else if(etype == Ekeyboard)
			switch(ev.kbdc){
			case 27:	/* escape */
			case '':
				exits(nil);
				break;
			case 61457:	/* left arrow */
				ndec(&viewing[mode], Jan, Dec);
				drawcalendar(mode, viewing[mode]);
				break;
			case 61458:	/* right arrow */
				ninc(&viewing[mode], Jan, Dec);
				drawcalendar(mode, viewing[mode]);
				break;
			}
	}
}