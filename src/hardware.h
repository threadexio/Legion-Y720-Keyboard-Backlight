#pragma once

#include <dirent.h>
#include <fcntl.h>
#include <linux/hidraw.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "log.h"

#define HIDRAW_PATH "/sys/class/hidraw"
#define HIDRAW_ID	"HID_ID=0018:0000048D:0000837A"

#define DEVICE_UEVENT_BUF_LEN 1024

typedef unsigned char u8;

typedef struct {
	u8 mode;
	u8 color;
	u8 brightness;
	u8 index;
} zone_t;

int hidiocsfeature(int fd, int len, const char* const data) {
	return ioctl(fd, HIDIOCSFEATURE(len), data);
}

int get_hidraw_dev() {
	DIR*		   d;
	struct dirent* dir;
	d = opendir(HIDRAW_PATH);

	if (! d) {
		logd(stderr, LOG_CRIT "Cannot open directory %s: %s", HIDRAW_PATH, ERR);
		return -1;
	}

	char* path_buf = (char*)malloc(PATH_MAX);
	if (! path_buf)
		return -1;

	char* device_uevent_buf = (char*)malloc(DEVICE_UEVENT_BUF_LEN);
	if (! device_uevent_buf)
		return -1;

	int device_uevent_fd = -1;

	int hidraw_device_fd = -1;
	while ((dir = readdir(d)) != NULL) {
		if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
			continue;

		memset(path_buf, 0, PATH_MAX);
		memset(device_uevent_buf, 0, DEVICE_UEVENT_BUF_LEN);

		snprintf(
			path_buf, PATH_MAX, HIDRAW_PATH "/%s/device/uevent", dir->d_name);

		close(device_uevent_fd);
		device_uevent_fd = open(path_buf, O_RDONLY);
		if (device_uevent_fd < 0) {
			logd(stderr, LOG_WARN "Cannot read file %s: %s", path_buf, ERR);
			continue;
		}

		if (read(device_uevent_fd,
				 device_uevent_buf,
				 DEVICE_UEVENT_BUF_LEN - 1) < 0) {
			logd(stderr,
				 LOG_WARN "Cannot read file %s: %s",
				 device_uevent_buf,
				 ERR);
			continue;
		}

		if (! strstr(device_uevent_buf, HIDRAW_ID))
			continue;

		snprintf(path_buf, PATH_MAX, "/dev/%s", dir->d_name);
		hidraw_device_fd = open(path_buf, O_WRONLY);
		break;
	}

	close(device_uevent_fd);

	free(path_buf);
	free(device_uevent_buf);
	return hidraw_device_fd;
}

int apply_zone(int hid_fd, zone_t* zone) {
	char buffer[] = {
		204, 0, zone->mode, zone->color, zone->brightness, zone->index};

	return hidiocsfeature(hid_fd, sizeof(buffer), buffer);
}

int apply_final(int hid_fd) {
	char buffer[] = {204, 9};

	return hidiocsfeature(hid_fd, sizeof(buffer), buffer);
}
