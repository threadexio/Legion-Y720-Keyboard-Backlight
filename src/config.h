#pragma once
#include <libconfig.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hardware.h"
#include "log.h"
#include "properties.h"

#define ZONE_NO 4

int read_config(const char *path,
				const char *profile_name,
				zone_t		_zones[ZONE_NO]) {
	config_t		  conf;
	config_setting_t *profile, *zones;
	config_init(&conf);

	if (! config_read_file(&conf, path)) {
		config_destroy(&conf);
		return -1;
	}

	char ref[PATH_MAX];
	snprintf(ref, PATH_MAX, "profiles.%s", profile_name);

	profile = config_lookup(&conf, ref);
	strcat(ref, ".zones");

	zones = config_lookup(&conf, ref);

	if (profile != NULL && zones != NULL) {
		const char *m;
		if (! config_setting_lookup_string(profile, "mode", &m)) {
			logd(stderr, LOG_CRIT "Missing field: mode in %s\n", profile_name);
			config_destroy(&conf);
			return -1;
		}

		for (int i = 0; i < ZONE_NO; i++) {
			config_setting_t *seg = config_setting_get_elem(zones, i);
			const char	   *bright;
			const char	   *c;

			if (! (config_setting_lookup_string(seg, "brightness", &bright) &&
				   config_setting_lookup_string(seg, "color", &c))) {
				logd(stderr,
					 LOG_CRIT "Missing field: color in %s\n",
					 profile_name);
				config_destroy(&conf);
				return -1;
			}

			_zones[i] = (zone_t) {
				.index		= i,
				.mode		= mode(m),
				.brightness = brightness(bright),
				.color		= color(c),
			};
		}
		return 0;
	}

	logd(stderr, LOG_CRIT "Cannot find profile: %s\n", profile_name);
	config_destroy(&conf);
	return -1;
}
