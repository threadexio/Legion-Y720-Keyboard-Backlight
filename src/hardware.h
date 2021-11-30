#ifndef _SRC_HARDWARE_H
#define _SRC_HARDWARE_H

#define HIDRAW_PATH "/sys/class/hidraw"
#define DEVICE_ID	"HID_ID=0003:0000048D:0000C100"
#define ZONE_NO		4

#define ZONE_BUF_LEN 6

typedef unsigned char u8;

typedef struct zone_t {
	u8 index;
	u8 mode;
	u8 color;
	u8 brightness;
} zone_t;

/**
 * @brief Find the correct hidraw device.
 *
 * @param dev A buffer to copy the device's name
 * @return int - 0 on success, -1 if not found
 */
int find_hidraw(char* dev);

int write_zone(int dev, zone_t zone);

int write_all(const char* dev, zone_t zones[ZONE_NO]);

#endif