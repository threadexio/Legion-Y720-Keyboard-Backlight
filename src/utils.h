#pragma once

#include <libconfig.h>
#include <string.h>
#include <unistd.h>

#define nullptr 0
#define PATH_MAX 4096

#define CPATH_MAX_LEN 255

#define REF_PREFIX	  "profiles."
#define REF_SUFFIX	  ".zones"
#define REF_MAX_LEN strlen(REF_PREFIX) + PNAME_MAX_LEN + strlen(REF_SUFFIX)

#define PNAME_MAX_LEN 18 // Profile name maximum length


#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"
#define RST "\e[0m"

typedef unsigned char bool;
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

bool fileExists(const char* file_path);

Segment_Conf *mkfullconf(const char *conf_path, char *ref);
