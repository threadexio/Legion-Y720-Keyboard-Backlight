#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "config.h"

#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"
#define RST "\e[0m"

int getColor (const char *color) {
	for (int i=0; i < 20; i++) {
		if (strcmp(color, colors[i]) == 0) {
			return i;
		}
	}
	return -1;
}

int getBrightness(const char *brightness) {
	for (int i=0; i < 6; i++) {
		if (strcmp(brightness, bright[i]) == 0) {
			return i;
		}
	}
	return -1;
}

int getMode (const char *mode) {
	for (int i=0; i < 5; i++) {
		if (strcmp(mode, modes[i]) == 0) {
			return i;
		}
	}
	return -1;
}

void findDevice(char *dev) {
	memset(dev, 0, sizeof(*dev));
	DIR *hidraw_d;
    struct dirent *dir;
    hidraw_d = opendir(HIDRAW_PATH);
    if (hidraw_d) {
		char buf[256];
        while ((dir = readdir(hidraw_d)) != NULL) {
			char path[256] = { 0 };
			strcpy(path, HIDRAW_PATH);
			strcat(path, dir->d_name);
			strcat(path, "/device/uevent");
            int fd = open( path , O_RDONLY);
			if (!fd) continue;
			read(fd, buf, 256);
			if (strstr(buf, HID_ID)) {
				strcpy(dev, dir->d_name);
				close(fd);
				break;
			}
        }
        closedir(hidraw_d);
    }
};

int writeSegment (int dev_fd, Segment_Conf conf) {
	byte buf[WR_BUF_SIZE] = { 204, 0, conf.mode, conf.color, conf.bright, conf.index };
	return write(dev_fd, buf, WR_BUF_SIZE) == WR_BUF_SIZE;
};

void writeConfig (const char *dev, Segment_Conf conf[SEGNO]) {
    char path[16] = {0};
    strcpy(path, "/dev/");
    strcat(path, dev);
	int dev_fd = open(path, O_WRONLY);
    
    byte final[WR_BUF_SIZE] = { 204, 9, 0, 0, 0, 0 };
	
    for (int i = 0; i < SEGNO; i++)
	{
		writeSegment(dev_fd, conf[i]);
	}
	
	write(dev_fd, final, WR_BUF_SIZE);
	
	close(dev_fd);
}
