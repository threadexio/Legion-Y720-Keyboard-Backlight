// https://github.com/threadexio/Legion-Y720-Keyboard-Backlight
#include <libconfig.h>
#include "utils.h"
#include "config.h"

#define DEFAULT_CONF "/etc/kbd-backlight/backlight.conf"

int main(int argc, char *argv[])
{
	char conf_path[256] = {0};
	if (argc < 2)
	{
		printf("Usage: %s%s%s [%sprofile name%s] {%sconfig file%s}\n", GRN, argv[0], RST, CYN, RST, YEL, RST);
		return EXIT_SUCCESS;
	}

	if (argc >= 3)
		strcpy(conf_path, argv[2]);
	else
		sprintf(conf_path, DEFAULT_CONF);

	if (geteuid() != 0)
	{
		fprintf(stderr, "%s[✘]%s Run me as root, otherwise nothing will happen\n", RED, RST);
		return EXIT_FAILURE;
	}

	char profile_ref[256] = {0};
	strcpy(profile_ref, "profiles.");
	strcat(profile_ref, argv[1]);

	Segment_Conf *conf = mkfullconf(conf_path, profile_ref);
	if (conf == nullptr) {
		return EXIT_FAILURE;
	}

	char hid_dev[8] = {0};
	findDevice(hid_dev);

	writeConfig(hid_dev, conf);
	printf("%s[✔]%s Loaded profile: %s\n", GRN, RST, argv[1]);

	free(conf);

	return EXIT_SUCCESS;
}
