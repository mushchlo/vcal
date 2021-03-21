void clearscreen(void);
void righttext(char*, Point, int, Font*);
void centertext(char*, Point, Font*);
void columntext(char**, Point, int, int, Font*);
int drawblocks(Rectangle, int, int, int, int, int, int);
void drawday(Yearscope);
void drawweek(Yearscope);
void drawmonth(Yearscope);
void drawyear(int);
void drawcalendar(int, Yearscope);

int isleapyear(int);
int before(Date, Date);
int leapdaysbetween(Date, Date);
int daysbetween(Date, Date);
int dayofweek(Date);
Date todate(int, int, int);
int fstdaymonth(Yearscope);
int fstdayweek(Yearscope);

int min(int, int);
int modulo(int, int);
Point addptint(Point, int);