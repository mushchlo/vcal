#include "vcal.h"

int
daysin(int m)
{
	int d[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	return d[m];
}

int
curmo(void)
{
	Tm *tm;

	tm = localtime(time(0));
	return tm->mon;
}

int
curyr(void)
{
	Tm *tm;

	tm = localtime(time(0));
	return tm->year+1900;
}

int
isleapyear(int y)
{
	if(y%4 == 0 && y%100 != 0
		|| y%400 == 0)
		return 1;
	return 0;
}

void
ndec(Yearscope *ctl, int min, int max)
{
	if(ctl->n > min)
		ctl->n--;
	else {
		ctl->n = max;
		ctl->year--; 
	}
}

void
ninc(Yearscope *ctl, int min, int max)
{
	if(ctl->n < max)
		ctl->n++;
	else {
		ctl->n = min;
		ctl->year++;
	}
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
		d += daysin(i) + (m == Feb && isleapyear(y));

	return (Date){y, d};
}

int
fstdaymonth(Yearscope t)
{
	return dayofweek(todate(t.year, t.n, 1));
}