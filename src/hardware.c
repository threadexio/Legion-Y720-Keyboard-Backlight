#include "hardware.h"

#include <dirent.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "log.h"

int find_hidraw(char *dev) {
	int ret = -1;

	struct dirent *dir;
	DIR			*sys = opendir(HIDRAW_PATH);

	while ((dir = readdir(sys)) != NULL) {
		char buf[PATH_MAX] = {0};

		// Skip ./ and ../
		if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
			continue;

		snprintf(
			buf, PATH_MAX, "%s/%s/device/uevent", HIDRAW_PATH, dir->d_name);

		int uevent;
		if ((uevent = open(buf, O_RDONLY)) < 0) {
			logd(stderr,
				 LOG_WARN "Cannot open device %s: %s\n",
				 dir->d_name,
				 strerror(errno));
			continue;
		}

		// We could make another buffer, but why allocate more stuff on the
		// stack?
		memset(buf, 0, PATH_MAX);

		if (read(uevent, buf, 256) < 0) {
			logd(stderr,
				 LOG_WARN "Cannot read device %s: %s\n",
				 dir->d_name,
				 strerror(errno));
			close(uevent);
			continue;
		} else {
			if (strstr(buf, DEVICE_ID)) {
				strcpy(dev, dir->d_name);
				ret = 0;
				close(uevent);
				break;
			}
		}
	}
	closedir(sys);
	return ret;
}

int write_zone(int dev, zone_t zone) {
	u8 buf[ZONE_BUF_LEN] = {
		204, 0, zone.mode, zone.color, zone.brightness, zone.index};
	return (write(dev, buf, ZONE_BUF_LEN) == ZONE_BUF_LEN) ? 0 : -1;
}

int write_all(const char *dev, zone_t *zones) {
	char dev_path[PATH_MAX + 1] = {0};
	snprintf(dev_path, PATH_MAX, "/dev/%s", dev);
	int fd;
	if ((fd = open(dev_path, O_WRONLY)) < 0) {
		logd(stderr, LOG_CRIT "Cannot open device: %s\n", strerror(errno));
		return -1;
	}

	u8 final[ZONE_BUF_LEN] = {204, 9, 0, 0, 0, 0};

	for (u8 i = 0; i < ZONE_NO; i++) {
		zone_t test = {.brightness = 1, .color = 4, .mode = 0, .index = i};
		if (write_zone(fd, test) < 0)
			logd(stderr, LOG_WARN "Error writing zone: %s\n", strerror(errno));
	}

	if (write(fd, final, ZONE_BUF_LEN) < 0) {
		logd(
			stderr, LOG_CRIT "Couldn't write to device: %s\n", strerror(errno));
		return -1;
	}

	return 0;
}