#pragma once
#define WR_BUF_SIZE 6
#define HIDRAW_PATH "/sys/class/hidraw/"
#define HID_ID "HID_NAME=ITE33D1:00 048D:837A"

#define SEGNO 4

typedef unsigned char byte;

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

typedef struct {
	int index;
    int mode;
    int color;
    int bright;
} Segment_Conf;
