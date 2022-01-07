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

#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#include <sys/utsname.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include "cfetch.h"

void add(struct wbuf *wb, const char *s, int len)
{
	char *new = realloc(wb->b, wb->len + len);
	if (new == NULL)
		return;

	memcpy(&new[wb->len], s, len);

	wb->b = new;
	wb->len += len;
}

void addf(struct wbuf *wb, int len, const char *fmt, ...)
{
	char buf[len];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	add(wb, buf, len);
}

void freeInf(struct fetch *info)
{
	free(info->name);
	free(info->os);
	free(info->kernel);
	free(info->uptime);
	free(info->shell);
	free(info->desktop);
}

void freeBuf(struct wbuf *wb)
{
	free(wb->b);
}

char *name(void)
{
	FILE *host = fopen("/etc/hostname", "r");
	char *line = NULL;
	size_t linecap = 0;

	if (!host)
		exit(1);

	if (getline(&line, &linecap, host) < 0)
	{
		free(line);
		exit(1);
	}

	fclose(host);

	return line;
}

char *os(void)
{
	FILE *os = fopen("/etc/os-release", "r");
	size_t linecap = 0;
	
	char *buf = malloc((size_t) BUFSIZE);
	char *line = NULL;
	int j = 0;

	if (!os)
		exit(1);

	while (getline(&line, &linecap, os) != -1)
	{
		if (strncmp(line, "PRETTY_NAME", 11) == 0)
			break;
		else if (line == NULL)
			exit(1);
	}

	for (int i = 13; i < strlen(line) - 1; i++)
	{
		buf[j] = line[i];
		j++;
	}

	buf[strlen(buf) - 1] = '\n';

	free(line);
	fclose(os);

	return buf;
}

char *kernel(void)
{
	struct utsname info;
	char *buf = malloc((size_t) BUFSIZE);

	uname(&info);
	snprintf(buf, BUFSIZE, "%s\n", info.release);

	return buf;
}

char *uptime(void)
{
	FILE *uptime = fopen("/proc/uptime", "r");
	size_t linecap = 0;

	char *line = NULL;
	char *buf = malloc((size_t) BUFSIZE);

	if (!uptime)
		exit(1);

	if (getdelim(&line, &linecap, ' ', uptime) < 0)
	{
		free(line);
		exit(1);
	}

	int time = strtol(line, NULL, 10);

	snprintf(buf, BUFSIZE, "%dD %dH %dM\n", time / 86400, time / 3600 % 24, time / 60 % 60);
	fclose(uptime);
	free(line);

	return buf;
}

char *shell(void)
{
	char *shell = getenv("SHELL");
	char *buf = malloc((size_t) BUFSIZE);

	for (int i = 0; i < 3; i++)
		strsep(&shell, "/");

	snprintf(buf, BUFSIZE, "%s\n", strsep(&shell, "\n"));

	return buf;
}

char *desktop(void)
{
	char *type = getenv("XDG_SESSION_TYPE");
	char *buf = malloc((size_t) BUFSIZE);

	if (!type)
	{
		if (getenv("WSLENV") != NULL)
			snprintf(buf, BUFSIZE, "%s\n", "WSL");
	}
	else if (strcmp(type, "tty") == 0)
	{
		if ((getenv("SSH_TTY")) != NULL)
			snprintf(buf, BUFSIZE, "%s\n", getenv("SSH_TTY"));
		else
			snprintf(buf, BUFSIZE, "%s\n", getenv("TTY"));
	}
	else
	{
		snprintf(buf, BUFSIZE, "%s\n", getenv("DESKTOP_SESSION"));
	}

	return buf;
}

int memoryU(void)
{
	return 0;
}

int memoryT(void)
{
	return 0;
}

int main(void)
{
	struct wbuf wb = WBUF_INIT;
	struct fetch info;

	info.name = name();
	info.os = os();
	info.kernel = kernel();
	info.uptime = uptime();
	info.shell = shell();
	info.desktop = desktop();
	info.memoryT = memoryT();
	info.memoryU = memoryU();

	addf(&wb, strlen(info.name) + 17, "\e[%dmName\e[0m :: %s", red, info.name);
	addf(&wb, strlen(info.os) + 15, "\e[%dmOS\e[0m :: %s", green, info.os);
	addf(&wb, strlen(info.kernel) + 19, "\e[%dmKernel\e[0m :: %s", yellow, info.kernel);
	addf(&wb, strlen(info.uptime) + 19, "\e[%dmUptime\e[0m :: %s", blue, info.uptime);
	addf(&wb, strlen(info.shell) + 18, "\e[%dmShell\e[0m :: %s", magenta, info.shell);
	addf(&wb, strlen(info.desktop) + 20, "\e[%dmDesktop\e[0m :: %s", cyan, info.desktop);

	write(STDOUT_FILENO, wb.b, wb.len);

	freeBuf(&wb);
	freeInf(&info);

	return 0;
}
