#ifndef _SRC_PROPERTIES_H
#define _SRC_PROPERTIES_H

typedef struct prop_t {
	const char* name;
	int			value;
} prop_t;

/**
 * @brief Array containing all the available colors.
 * @see properties.c
 */
extern const prop_t colors[];

/**
 * @brief Array containing all the available brightness values.
 * @see properties.c
 */
extern const prop_t brightness[];

/**
 * @brief Array containing all the available modes.
 * @see properties.c
 */
extern const prop_t modes[];

/**
 * @brief Get color value from name.
 * @see colors
 *
 * @param x
 * @return int - The color value, or -1 if color doesn't exist
 */
int color(const char* x);

/**
 * @brief Get brightness value from name.
 * @see brightness
 *
 * @param x
 * @return int - The brightness value, or -1 if color doesn't exist
 */
int bright(const char* x);

/**
 * @brief Get mode value from name.
 * @see modes
 *
 * @param x
 * @return int - The mode value, or -1 if color doesn't exist
 */
int mode(const char* x);

#endif