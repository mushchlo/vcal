	/* vcal.c */
void		drawday(Yearscope);
void		drawweek(Yearscope);
void		drawmonth(Yearscope);
void		drawyear(int);
void		drawcalendar(int, Yearscope);

	/* date.c */
int			daysin(int);
int			curmo(void);
int			curyr(void);
int			isleapyear(int);
void		ndec(Yearscope*, int, int);
void		ninc(Yearscope*, int, int);
int			before(Date, Date);
int			leapdaysbetween(Date, Date);
int			daysbetween(Date, Date);
int			dayofweek(Date);
Date		todate(int, int, int);
int			fstdaymonth(Yearscope);

	/* arith.c */
int			min(int, int);
int			modulo(int, int);
Point		addptint(Point, int);
void		ptiterate(Point*, int);
Point		center(Rectangle);
Rectangle	centerrect(Point, int, int);
int			maxstrwidth(char**, Font*);

	/* draw.c */
void		clearscreen(void);
Threebutton	newbutton(void);
void		holdonbutton(int, Mouse);
void		righttext(char*, Point, int, Font*);
void		centertext(char*, Point, Font*);
void		columntext(char**, Point, int, int, Font*);
void		drawchev(Point, int, int);
Rectangle	drawbutton(Point, Point, char*, int, int, Font*);
Rectangle	drawblocks(Rectangle, int, int, int, int, int, int);