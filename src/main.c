// https://github.com/threadexio/Legion-Y720-Keyboard-Backlight
#include <libconfig.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "hw.h"
#include "utils.h"

#define CONF_PATH_MAX_LEN 256
#define CONF_REF_MAX_LEN 32

#ifdef _DEBUG
#define DEFAULT_CONF "files/backlight.conf"
#else
#define DEFAULT_CONF "/etc/kbd-backlight/backlight.conf"
#endif

int main(int argc, char *argv[]) {
	char conf_path[CONF_PATH_MAX_LEN + 1] = {0};
	if (argc < 2) {
		printf("Usage: %s%s%s [%sprofile name%s] {%sconfig file%s}\n", GRN, argv[0], RST, CYN, RST, YEL, RST);
		return EXIT_SUCCESS;
	}

#ifndef _DEBUG
	if (geteuid() != 0) {
		fprintf(stderr, "%s[✘]%s Run me as root, otherwise nothing will happen\n", RED, RST);
		return EXIT_FAILURE;
	}
#endif

	const char *profile_name = argv[1];
	const char *config_file = argv[2];

	snprintf(conf_path, CONF_PATH_MAX_LEN, DEFAULT_CONF);
	if (config_file) {
		snprintf(conf_path, CONF_PATH_MAX_LEN, config_file);
	}

	char ref[CONF_REF_MAX_LEN + 1] = {0};
	const char *prefix = "profiles";
	snprintf(ref, CONF_PATH_MAX_LEN, "%s.%s", prefix, profile_name);

	Segment_Conf *conf = mkfullconf(conf_path, ref);
	if (conf == nullptr) {
		free(conf);
		return EXIT_FAILURE;
	}

	char hid_dev[8] = {0};
	findDevice(hid_dev);

	writeConfig(hid_dev, conf);
	printf("%s[✔]%s Loaded profile: %s\n", GRN, RST, profile_name);

	free(conf);

	return EXIT_SUCCESS;
}
