#pragma once

#include <libconfig.h>
#include <string.h>

#define nullptr 0
#define PATH_MAX 4096

#define CONF_PATH_MAX_LEN 256
#define CONF_REF_PREFIX	  "profiles."
#define CONF_REF_SUFFIX	  ".zones"
#define CONF_PROF_MAX_LEN 18
#define CONF_REF_MAX_LEN  strlen(CONF_REF_PREFIX) + CONF_PROF_MAX_LEN + strlen(CONF_REF_SUFFIX)

#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"
#define RST "\e[0m"

typedef unsigned char byte;
typedef struct {
	int index;
	int mode;
	int color;
	int bright;
} Segment_Conf;

extern const char *colors[];

extern const char *bright[];

extern const char *modes[];

// Debug purposes only
void PrintSegConf(Segment_Conf *config);

int getColor(const char *color);

int getBrightness(const char *brightness);

int getMode(const char *mode);

Segment_Conf *mkfullconf(const char *conf_path, char *ref);
