/*
* cfetch - A shitty fetch
* Copyright (C) 2021 ItzAfroBoy
* 
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef __CFETCH_H__
#define __CFETCH_H__

#define WBUF_INIT { NULL, 0 }
#define BUFSIZE 80

struct wbuf
{
	char *b;
	int len;
};

struct fetch
{
	char *name;
	char *os;
	char *kernel;
	char *uptime;
	char *shell;
	char *desktop;
	int memoryT;
	int memoryU;
};

enum colors
{
	black = 30,
	red,
	green,
	yellow,
	blue,
	magenta,
	cyan,
	white,
	gray = 90,
	blackBG = 40,
	redBG,
	greenBG,
	yellowBG,
	blueBG,
	magentaBG,
	cyanBG,
	whiteBG,
	grayBG = 100
};

void add(struct wbuf *wb, const char *s, int len);
void addf(struct wbuf *wb, int len, const char *fmt, ...);

void freeBuf(struct wbuf *wb);
void freeInf(struct fetch *info);

char *name(void);
char *os(void);
char *kernel(void);
char *uptime(void);
char *shell(void);
char *desktop(void);
int memoryU(void);
int memoryT(void);

#endif // __CFETCH_H__
