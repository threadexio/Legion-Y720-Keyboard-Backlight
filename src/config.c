#include "config.h"

#include <libconfig.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hardware.h"
#include "log.h"
#include "properties.h"

int read_config(const char *path, const char *pname, zone_t *_zones) {
	config_t		  conf;
	config_setting_t *profile, *zones;
	config_init(&conf);

	if (! config_read_file(&conf, path)) {
		logd(stderr,
			 LOG_CRIT "%s @ %d - %s\n",
			 path,
			 config_error_line(&conf),
			 config_error_text(&conf));
		config_destroy(&conf);
		return -1;
	}

	char ref[PATH_MAX + 1];
	sprintf(ref, "profiles.%s", pname);

	profile = config_lookup(&conf, ref);
	strcat(ref, ".zones");

	zones = config_lookup(&conf, ref);

	if (profile != NULL && zones != NULL) {
		const char *m;
		if (! config_setting_lookup_string(profile, "mode", &m)) {
			logd(stderr, LOG_CRIT "Missing field: mode in %s\n", pname);
			config_destroy(&conf);
			return -1;
		}

		if (mode(m) == -1) {
			logd(stderr, LOG_CRIT "Invalid mode found: %s\n", m);
			config_destroy(&conf);
			return -1;
		}

		zone_t *_zones = (zone_t *)malloc(sizeof(zone_t) * ZONE_NO);

		for (int i = 0; i < ZONE_NO; i++) {
			config_setting_t *seg = config_setting_get_elem(zones, i);
			const char	   *brightness;
			const char	   *c;

			if (! (config_setting_lookup_string(
					   seg, "brightness", &brightness) &&
				   config_setting_lookup_string(seg, "color", &c))) {
				logd(stderr, LOG_CRIT "Missing field: color in %s\n", pname);
				config_destroy(&conf);
				free(_zones);
				return -1;
			}

			if (bright(brightness) == -1) {
				logd(stderr,
					 LOG_CRIT "Invalid brightness value: %s\n",
					 brightness);
				config_destroy(&conf);
				free(_zones);
				return -1;
			}

			if (color(c) == -1) {
				logd(stderr, LOG_CRIT "Invalid color: %s\n", c);
				config_destroy(&conf);
				free(_zones);
				return -1;
			}
			_zones[i] = (zone_t) {
				.index		= i,
				.mode		= mode(m),
				.brightness = bright(brightness),
				.color		= color(c),
			};
		}
		return 0;
	}

	logd(stderr, LOG_CRIT "Cannot find profile: %s\n", pname);
	config_destroy(&conf);
	return -1;
}