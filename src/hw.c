#include "hw.h"

#include <dirent.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void findDevice(char *dev) {
	memset(dev, 0, sizeof(*dev));
	DIR *		   hidraw_d;
	struct dirent *dir;
	hidraw_d = opendir(HIDRAW_PATH);
	if (hidraw_d) {
		char buf[256];
		while ((dir = readdir(hidraw_d)) != NULL) {
			char path[256] = {0};
			strcpy(path, HIDRAW_PATH);
			strcat(path, dir->d_name);
			strcat(path, "/device/uevent");
			int fd = open(path, O_RDONLY);
			if (! fd)
				continue;
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

int writeSegment(int dev_fd, Segment_Conf conf) {
	byte buf[WR_BUF_SIZE] = {
		204, 0, conf.mode, conf.color, conf.bright, conf.index};
	return write(dev_fd, buf, WR_BUF_SIZE) == WR_BUF_SIZE;
};

void writeConfig(const char *dev, Segment_Conf conf[SEGNO]) {
	char path[16] = {0};
	strcpy(path, "/dev/");
	strcat(path, dev);
	int dev_fd = open(path, O_WRONLY);

	byte final[WR_BUF_SIZE] = {204, 9, 0, 0, 0, 0};

	for (int i = 0; i < SEGNO; i++) writeSegment(dev_fd, conf[i]);

	write(dev_fd, final, WR_BUF_SIZE);

	close(dev_fd);
}