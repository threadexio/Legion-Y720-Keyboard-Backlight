#include "log.h"

int logd(FILE *out, const char *fmt, ...) {
	int		ret;
	va_list args;

	va_start(args, fmt);
	ret = vfprintf(out, fmt, args);
	va_end(args);

	return ret;
}