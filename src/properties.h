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
	{.key = "Crimson", .val = 0},
	{.key = "Torch Red", .val = 1},
	{.key = "Hollywood Cerise", .val = 2},
	{.key = "Magent", .val = 3},
	{.key = "Electric Violet", .val = 4},
	{.key = "Electric Violet 2", .val = 5},
	{.key = "Blue", .val = 6},
	{.key = "Blue Ribbon", .val = 7},
	{.key = "Azure Radiance", .val = 8},
	{.key = "Cyan", .val = 9},
	{.key = "Spring Green", .val = 10},
	{.key = "Spring Green 2", .val = 11},
	{.key = "Green", .val = 12},
	{.key = "Bright Green", .val = 13},
	{.key = "Lime", .val = 14},
	{.key = "Yellow", .val = 15},
	{.key = "Web Orange", .val = 16},
	{.key = "International Orange", .val = 17},
	{.key = "White", .val = 18},
	{.key = "No Color", .val = 19},
};
LOOKUP(color);

static KV brightness_table[] = {
	{.key = "Off", .val = 0},
	{.key = "Low", .val = 1},
	{.key = "Medium", .val = 2},
	{.key = "High", .val = 3},
	{.key = "Ultra", .val = 4},
	{.key = "Enough", .val = 5},
};
LOOKUP(brightness);

static KV mode_table[] = {
	{.key = "Heartbeat", .val = 0},
	{.key = "Breath", .val = 1},
	{.key = "Smooth", .val = 2},
	{.key = "Always On", .val = 3},
	{.key = "Wave", .val = 4},
};
LOOKUP(mode);
