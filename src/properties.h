#pragma once
#include <string.h>

#include "hardware.h"

#define SIZE(x) (sizeof(x) / sizeof(x[0]))

#define LOOKUP(c)                                                              \
	u8 c(const char* c) {                                                      \
		for (int i = 0; i < SIZE(c##_table); i++) {                            \
			if (strcmp(c##_table[i].key, c) == 0) {                            \
				return c##_table[i].val;                                       \
			}                                                                  \
		}                                                                      \
		return 0x0;                                                            \
	}

typedef struct {
	const char* key;
	u8			val;
} KV;

static KV color_table[] = {
	{.key = "CRIMSON", .val = 0},
	{.key = "TORCH_RED", .val = 1},
	{.key = "HOLLYWOOD_CERISE", .val = 2},
	{.key = "MAGENTA", .val = 3},
	{.key = "ELECTRIC_VIOLET", .val = 4},
	{.key = "ELECTRIC_VIOLET_2", .val = 5},
	{.key = "BLUE", .val = 6},
	{.key = "BLUE_RIBBON", .val = 7},
	{.key = "AZURE_RADIANCE", .val = 8},
	{.key = "CYAN", .val = 9},
	{.key = "SPRING_GREEN", .val = 10},
	{.key = "SPRING_GREEN_2", .val = 11},
	{.key = "GREEN", .val = 12},
	{.key = "BRIGHT_GREEN", .val = 13},
	{.key = "LIME", .val = 14},
	{.key = "YELLOW", .val = 15},
	{.key = "WEB_ORANGE", .val = 16},
	{.key = "INTERNATIONAL_ORANGE", .val = 17},
	{.key = "WHITE", .val = 18},
	{.key = "NOCOLOR", .val = 19},
};
LOOKUP(color);

static KV brightness_table[] = {
	{.key = "OFF", .val = 0},
	{.key = "LOW", .val = 1},
	{.key = "MEDIUM", .val = 2},
	{.key = "HIGH", .val = 3},
	{.key = "ULTRA", .val = 4},
	{.key = "ENOUGH", .val = 5},
};
LOOKUP(brightness);

static KV mode_table[] = {
	{.key = "HEARTBEAT", .val = 0},
	{.key = "BREATH", .val = 1},
	{.key = "SMOOTH", .val = 2},
	{.key = "ALWAYS_ON", .val = 3},
	{.key = "WAVE", .val = 4},
};
LOOKUP(mode);
