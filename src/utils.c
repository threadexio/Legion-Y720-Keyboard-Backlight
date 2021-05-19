#include "utils.h"

#include <stdlib.h>
#include <string.h>

const char *colors[] = {
	"CRIMSON", "TORCH_RED",		  "HOLLYWOOD_CERISE",
	"MAGENTA", "ELECTRIC_VIOLET", "ELECTRIC_VIOLET_2",
	"BLUE",	   "BLUE_RIBBON",	  "AZURE_RADIANCE",
	"CYAN",	   "SPRING_GREEN",	  "SPRING_GREEN_2",
	"GREEN",   "BRIGHT_GREEN",	  "LIME",
	"YELLOW",  "WEB_ORANGE",	  "INTERNATIONAL_ORANGE",
	"WHITE",   "NO_COLOR",
};

const char *bright[] = {
	"OFF",
	"LOW",
	"MEDIUM",
	"HIGH",
	"ULTRA",
	"ENOUGH",
};

const char *modes[] = {
	"HEARTBEAT",
	"BREATH",
	"SMOOTH",
	"ALWAYS_ON",
	"WAVE",
};

void PrintSegConf(Segment_Conf *c) {
	printf("index:\t%d\n", c->index);
	printf("mode:\t%d\n", c->mode);
	printf("color:\t%d\n", c->color);
	printf("bright:\t%d\n", c->bright);
}

int getColor(const char *color) {
	for (int i = 0; i < 20; i++) {
		if (strcmp(color, colors[i]) == 0) {
			return i;
		}
	}
	return -1;
}

int getBrightness(const char *brightness) {
	for (int i = 0; i < 6; i++) {
		if (strcmp(brightness, bright[i]) == 0) {
			return i;
		}
	}
	return -1;
}

int getMode(const char *mode) {
	for (int i = 0; i < 5; i++) {
		if (strcmp(mode, modes[i]) == 0) {
			return i;
		}
	}
	return -1;
}

Segment_Conf *mkfullconf(const char *conf_path, char *ref) {
	config_t		  conf;
	config_setting_t *profile, *zones;
	config_init(&conf);

	if (! config_read_file(&conf, conf_path)) {
		fprintf(stderr,
				"%s[✘]%s %s @ %d - %s\n",
				RED,
				RST,
				conf_path,
				config_error_line(&conf),
				config_error_text(&conf));
		config_destroy(&conf);
		return nullptr;
	}

	profile = config_lookup(&conf, ref);
	strcat(ref, CONF_REF_SUFFIX);
	zones = config_lookup(&conf, ref);

	if (profile != NULL && zones != NULL) {
		const char *mode;
		if (! config_setting_lookup_string(profile, "mode", &mode)) {
			fprintf(stderr, "%s[✘]%s Missing fields\n", RED, RST);
			config_destroy(&conf);
			return nullptr;
		}

		if (getMode(mode) == -1) {
			fprintf(stderr, "%s[✘]%s Invalid mode found: %s\n", RED, RST, mode);
			config_destroy(&conf);
			return nullptr;
		}

		int			  count = config_setting_length(zones);
		Segment_Conf *fullconf =
			(Segment_Conf *)malloc(sizeof(Segment_Conf) * count);

		for (int i = 0; i < count; i++) {
			config_setting_t *seg = config_setting_get_elem(zones, i);
			const char *	  brightness;
			const char *	  color;

			if (! (config_setting_lookup_string(
					   seg, "brightness", &brightness) &&
				   config_setting_lookup_string(seg, "color", &color))) {
				fprintf(stderr, "%s[✘]%s Missing fields\n", RED, RST);
				config_destroy(&conf);
				free(fullconf);
				return nullptr;
			}

			if (getBrightness(brightness) == -1) {
				fprintf(stderr,
						"%s[✘]%s Invalid brightness found: %s\n",
						RED,
						RST,
						brightness);
				config_destroy(&conf);
				free(fullconf);
				return nullptr;
			}

			if (getColor(color) == -1) {
				fprintf(stderr,
						"%s[✘]%s Invalid color found: %s\n",
						RED,
						RST,
						color);
				config_destroy(&conf);
				free(fullconf);
				return nullptr;
			}
			fullconf[i] = (Segment_Conf) {
				.index	= i,
				.mode	= getMode(mode),
				.bright = getBrightness(brightness),
				.color	= getColor(color),
			};
		}
		return fullconf;
	}
	fprintf(stderr, "%s[✘]%s Cannot find profile\n", RED, RST);
	config_destroy(&conf);
	return nullptr;
}