#include <u.h>
#include <libc.h>
#include <draw.h>
#include <event.h>
#include <keyboard.h>

#define SWAP(x, y, T) do{ T SWAPTMP = x; x = y; y = SWAPTMP; } while(0)
#define defont display->defaultfont

enum { Day, Week, Month, Year };
enum { Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec };
enum { Noclick, Left, Mid, Right = 4 };

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

struct Threebutton {
	Rectangle	region;
	void (*action[5])(Mouse *m, Menu *mn, Yearscope *ctl, int mode);
};
typedef struct Threebutton Threebutton;

#include "fns.h"