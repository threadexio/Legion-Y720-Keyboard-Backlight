#include <linux/limits.h>
#include <sys/stat.h>
#include <fnmatch.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>


#include "utils.h"

const char *colors[] = {
	"CRIMSON",
	"TORCH_RED",
	"HOLLYWOOD_CERISE",
	"MAGENTA",
	"ELECTRIC_VIOLET",
	"ELECTRIC_VIOLET_2",
	"BLUE",
	"BLUE_RIBBON",
	"AZURE_RADIANCE",
	"CYAN",
	"SPRING_GREEN",
	"SPRING_GREEN_2",
	"GREEN",
	"BRIGHT_GREEN",
	"LIME",
	"YELLOW",
	"WEB_ORANGE",
	"INTERNATIONAL_ORANGE",
	"WHITE",
	"NO_COLOR",
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

int getColor(const char *color)
{
	for (int i = 0; i < 20; i++)
	{
		if (strcmp(color, colors[i]) == 0)
		{
			return i;
		}
	}
	return -1;
}

int getBrightness(const char *brightness)
{
	for (int i = 0; i < 6; i++)
	{
		if (strcmp(brightness, bright[i]) == 0)
		{
			return i;
		}
	}
	return -1;
}

int getMode(const char *mode)
{
	for (int i = 0; i < 5; i++)
	{
		if (strcmp(mode, modes[i]) == 0)
		{
			return i;
		}
	}
	return -1;
}

Segment_Conf *mkfullconf(const char *conf_path, const char *ref)
{
	config_t conf;
	config_setting_t *profile;
	config_init(&conf);

	if (!config_read_file(&conf, conf_path))
	{
		fprintf(stderr, "%s[✘]%s Cannot read config file: %s\n", RED, RST, conf_path);
		config_destroy(&conf);
		return nullptr;
	}

	profile = config_lookup(&conf, ref);

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
				fprintf(stderr, "%s[✘]%s Missing fields\n", RED, RST);
				return nullptr;
			}

			if (getMode(mode) == -1)
			{
				fprintf(stderr, "%s[✘]%s Invalid mode found: %s\n", RED, RST, mode);
				return nullptr;
			}

			if (getBrightness(brightness) == -1)
			{
				fprintf(stderr, "%s[✘]%s Invalid brightness found: %s\n", RED, RST, brightness);
				return nullptr;
			}

			if (getColor(color) == -1)
			{
				fprintf(stderr, "%s[✘]%s Invalid color found: %s\n", RED, RST, color);
				return nullptr;
			}
			fullconf[i] = (Segment_Conf){
				.index = i,
				.mode = getMode(mode),
				.bright = getBrightness(brightness),
				.color = getColor(color),
			};
		}
		return fullconf;
	}
	fprintf(stderr, "%s[✘]%s Cannot find profile\n", RED, RST);
	return nullptr;
}

const char **include_func(config_t *config,
								 const char *include_dir,
								 const char *path,
								 const char **error)
{
	char *p;
	DIR *dp;
	struct dirent *dir_entry;
	struct stat stat_buf;
	char include_path[PATH_MAX + 1];
	size_t include_path_len = 0;
	char file_path[PATH_MAX + 1];
	char **result = NULL;
	char **result_next = result;
	int result_count = 0;
	int result_capacity = 0;

	*include_path = 0;

	if (*path != '/')
	{
		if (include_dir)
		{
			strcat(include_path, include_dir);
			include_path_len += strlen(include_dir);
		}
	}

	//strcpy(p, strrchr(path, '/'));
	p = strrchr(path, '/');
	if (p > path)
	{
		int len = p - path;

		if ((include_path_len > 0) && (*(include_path + include_path_len - 1) != '/'))
		{
			strcat(include_path, "/");
			++include_path_len;
		}

		strncat(include_path, path, len);
		include_path_len += len;
	}

	if (include_path_len == 0)
	{
		strcpy(include_path, ".");
		include_path_len = 1;
	}

	dp = opendir(include_path);
	if (dp)
	{
		while ((dir_entry = readdir(dp)) != NULL)
		{
			snprintf(file_path, PATH_MAX, "%s/%s", include_path, dir_entry->d_name);
			if (lstat(file_path, &stat_buf) != 0)
				continue;
			if (!S_ISREG(stat_buf.st_mode))
				continue;
			if (fnmatch(path, file_path, FNM_PATHNAME) != 0)
				continue;

			if (result_count == result_capacity)
			{
				result_capacity += 16;
				result = (char **)realloc(result, (result_capacity + 1) * sizeof(char *));
				result_next = result + result_count;
			}

			*result_next = strdup(file_path);
			++result_next;
			++result_count;
		}
		closedir(dp);
	}

	*result_next = NULL;

	return ((const char **)result);
}
