#ifndef _SRC_LOG_H
#define _SRC_LOG_H

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>

#define LOG_WARN "\e[0;33m  ⚠  \e[0m"
#define LOG_SUCC "\e[0;32m  ✔  \e[0m"
#define LOG_CRIT "\e[0;31m  ✘  \e[0m"

int logd(FILE *out, const char *fmt, ...);

#endif