#pragma once

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>

#define ERR strerror(errno)

#define LOG_WARN "\e[0;33m  ⚠  \e[0m"
#define LOG_SUCC "\e[0;32m  ✔  \e[0m"
#define LOG_CRIT "\e[0;31m  ✘  \e[0m"

int logd(FILE *out, const char *fmt, ...) {
	int		ret;
	va_list args;

	va_start(args, fmt);
	ret = vfprintf(out, fmt, args);
	va_end(args);

	fwrite("\n", 1, 1, out);

	return ret;
}
