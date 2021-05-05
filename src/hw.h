#pragma once

#include "utils.h"

#define WR_BUF_SIZE 6
#define HIDRAW_PATH "/sys/class/hidraw/"
#define HID_ID "HID_NAME=ITE33D1:00 048D:837A"

#define SEGNO 4

void findDevice(char *dev);

int writeSegment(int dev_fd, Segment_Conf conf);

void writeConfig(const char *dev, Segment_Conf conf[SEGNO]);
