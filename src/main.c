#include <linux/limits.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "config.h"
#include "hardware.h"
#include "log.h"
#include "properties.h"

#ifdef _DEBUG
#define SYSTEM_CONF	  "files/backlight.conf"
#define PERSONAL_CONF ".config/kbd-backlight.conf"
#else
#define SYSTEM_CONF	  "/etc/kbd-backlight/backlight.conf"
#define PERSONAL_CONF ".config/kbd-backlight.conf"
#endif

int main(int argc, char *argv[]) {
	if (argc < 2) {
		logd(stdout, "Usage: %s [profile name] {config file}\n", argv[0]);
		exit(EXIT_SUCCESS);
	}
	const char *pname = argv[1];

	char conf_path[PATH_MAX + 1] = {0};

	struct passwd *pw = getpwuid(getuid());
	snprintf(conf_path, PATH_MAX, "%s/%s", pw->pw_dir, PERSONAL_CONF);

	if (argc > 2) {
		if (access(argv[2], F_OK) == 0) {
			strncpy(conf_path, argv[2], PATH_MAX);
		} else {
			logd(stderr, LOG_CRIT "Cannot open config file at: %s\n", argv[2]);
			exit(EXIT_FAILURE);
		}
	} else if (access(conf_path, F_OK) == 0) {
		// if we find the personal config dont do anything because
		// its already in the buffer
	} else if (access(SYSTEM_CONF, F_OK) == 0) {
		strcpy(conf_path, SYSTEM_CONF);
	} else {
		logd(stderr, LOG_CRIT "Cannot find any config file\n");
		exit(EXIT_FAILURE);
	}

	zone_t *zones;
	if (read_config(conf_path, pname, zones) < 0)
		exit(EXIT_FAILURE);

	char dev[16] = {0};
	if (find_hidraw(dev) < 0)
		exit(EXIT_FAILURE);

	if (write_all(dev, zones) < 0)
		exit(EXIT_FAILURE);

	logd(stdout, LOG_SUCC "Loaded profile: %s\n", pname);
}