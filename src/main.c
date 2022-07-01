#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>

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

int main(int argc, char* argv[]) {
	if (argc < 2) {
		logd(stdout, "Usage: %s [profile name] {config file}\n", argv[0]);
		exit(EXIT_SUCCESS);
	}

	zone_t zones[ZONE_NO];

	const char* profile_name = argv[1];

	char* conf_file = (char*)malloc(PATH_MAX);

	if (argc < 3) {
		struct passwd* pw = getpwuid(getuid());
		snprintf(conf_file, PATH_MAX, "%s/%s", pw->pw_dir, PERSONAL_CONF);

		if (access(conf_file, R_OK) < 0) {
			snprintf(conf_file, PATH_MAX, SYSTEM_CONF);
			if (access(conf_file, R_OK) < 0) {
				logd(stderr, LOG_CRIT "Cannot find readable config file");
				exit(1);
			}
		}
	} else {
		snprintf(conf_file, PATH_MAX, "%s", argv[2]);
	}

	if (read_config(conf_file, profile_name, zones) < 0)
		exit(1);

	free(conf_file);

	int hidraw_fd = get_hidraw_dev();
	if (hidraw_fd < 0) {
		logd(stderr, LOG_CRIT "Cannot use hidraw device: %s", ERR);
		exit(1);
	}

	for (int i = 0; i < ZONE_NO; i++) {
		printf("color = %d, brightness = %d, mode = %d, index = %d\n",
			   zones[i].color,
			   zones[i].brightness,
			   zones[i].mode,
			   zones[i].index);

		if (apply_zone(hidraw_fd, &zones[i]) < 0) {
			logd(stderr,
				 LOG_CRIT "Cannot apply zone %d: %s",
				 zones[i].index,
				 ERR);
			exit(1);
		}
	}

	if (apply_final(hidraw_fd) < 0) {
		logd(stderr, LOG_CRIT "Cannot finish LED update: %s", ERR);
		exit(1);
	}

	logd(stderr, LOG_SUCC "Loaded profile: %s", profile_name);
}
