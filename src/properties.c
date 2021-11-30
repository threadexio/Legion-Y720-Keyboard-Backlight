#include "properties.h"

#include <string.h>

const prop_t colors[] = {
	{"CRIMSON", 0}, {"TORCH_RED", 1},		{"HOLLYWOOD_CERISE", 2},
	{"MAGENTA", 3}, {"ELECTRIC_VIOLET", 4}, {"ELECTRIC_VIOLET_2", 5},
	{"BLUE", 6},	{"BLUE_RIBBON", 7},		{"AZURE_RADIANCE", 8},
	{"CYAN", 9},	{"SPRING_GREEN", 10},	{"SPRING_GREEN_2", 11},
	{"GREEN", 12},	{"BRIGHT_GREEN", 13},	{"LIME", 14},
	{"YELLOW", 15}, {"WEB_ORANGE", 16},		{"INTERNATIONAL_ORANGE", 17},
	{"WHITE", 18},	{"NO_COLOR", 19},
};

const prop_t brightness[] = {
	{"OFF", 0},
	{"LOW", 1},
	{"MEDIUM", 2},
	{"HIGH", 3},
	{"ULTRA", 4},
	{"ENOUGH", 5},
};

const prop_t modes[] = {
	{"HEARTBEAT", 0},
	{"BREATH", 1},
	{"SMOOTH", 2},
	{"ALWAYS_ON", 3},
	{"WAVE", 4},
};

int color(const char* x) {
	for (int i = 0; i < (sizeof(colors) / sizeof(colors[0])); i++)
		if (strcmp(x, colors[i].name) == 0)
			return colors[i].value;
	return -1;
}

int bright(const char* x) {
	for (int i = 0; i < (sizeof(brightness) / sizeof(brightness[0])); i++)
		if (strcmp(x, brightness[i].name) == 0)
			return brightness[i].value;
	return -1;
}

int mode(const char* x) {
	for (int i = 0; i < (sizeof(modes) / sizeof(modes[0])); i++)
		if (strcmp(x, modes[i].name) == 0)
			return modes[i].value;
	return -1;
}