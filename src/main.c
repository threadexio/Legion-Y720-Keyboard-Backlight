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

	config_t conf;
	config_setting_t *profile;
	config_init(&conf);

	if (!config_read_file(&conf, conf_path))
	{
		fprintf(stderr, "%s[✘]%s Cannot read config file: %s\n", RED, RST, conf_path);
		config_destroy(&conf);
		return EXIT_FAILURE;
	}

	profile = config_lookup(&conf, profile_ref);

	if (profile != NULL)
	{
		int count = config_setting_length(profile);

		Segment_Conf *fullconf = (Segment_Conf *)malloc(sizeof(Segment_Conf) * count);

		for (int i = 0; i < count; i++)
		{
			config_setting_t *seg = config_setting_get_elem(profile, i);
			const char *mode;
			const char *brightness;
			const char *color;

			if (!(config_setting_lookup_string(seg, "mode", &mode) && config_setting_lookup_string(seg, "brightness", &brightness) && config_setting_lookup_string(seg, "color", &color)))
			{
				fprintf(stderr, "%s[✘]%s Missing fields: index/color\n", RED, RST);
				return EXIT_FAILURE;
			}

			Segment_Conf segment;

			segment.index = i;

			if (getMode(mode) == -1)
			{
				fprintf(stderr, "%s[✘]%s Invalid mode found: %s\n", RED, RST, mode);
				return EXIT_FAILURE;
			}
			segment.mode = getMode(mode);

			if (getBrightness(brightness) == -1)
			{
				fprintf(stderr, "%s[✘]%s Invalid brightness found: %s\n", RED, RST, brightness);
				return EXIT_FAILURE;
			}
			segment.bright = getBrightness(brightness);

			if (getColor(color) == -1)
			{
				fprintf(stderr, "%s[✘]%s Invalid color found: %s\n", RED, RST, color);
				return EXIT_FAILURE;
			}
			segment.color = getColor(color);

			fullconf[i] = segment;
		}

		char hid_dev[8] = {0};
		findDevice(hid_dev);

		writeConfig(hid_dev, fullconf);
		printf("%s[✔]%s Loaded profile: %s\n", GRN, RST, argv[1]);

		free(fullconf);

		return EXIT_SUCCESS;
	}
	fprintf(stderr, "%s[✘]%s Cannot find profile: %s\n", RED, RST, argv[1]);
	return EXIT_FAILURE;
}
