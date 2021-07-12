// https://github.com/threadexio/Legion-Y720-Keyboard-Backlight
#include <libconfig.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "hw.h"
#include "user.h"
#include "utils.h"

#define CHOME_PATH ".config/kbd-backlight.conf"
#define GROUP	   "kbd-backlight"

#ifdef _DEBUG
#define DEFAULT_CONF "files/backlight.conf"
#else
#define DEFAULT_CONF "/etc/kbd-backlight/backlight.conf"
#endif

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Usage: %s%s%s [%sprofile name%s] {%sconfig file%s}\n",
			   GRN,
			   argv[0],
			   RST,
			   CYN,
			   RST,
			   YEL,
			   RST);
		exit(EXIT_SUCCESS);
	}

	const char *c_Pname		  = argv[1];
	const char *c_AltConfPath = argv[2];

	char c_FinalConfPath[CPATH_MAX_LEN + 1] = {0};

	// Only allow members of GROUP and root to change the backlight
	struct passwd pw = getUserPw();
	if (! isUserInGroup(&pw, GROUP) && ! isUserRoot()) {
		fprintf(stderr, "%s[✘]%s User not in the %s group\n", RED, RST, GROUP);
		exit(EXIT_FAILURE);
	}

	// Another config file was specified from argv
	if (c_AltConfPath) {
		if (fileExists(c_AltConfPath))
			snprintf(c_FinalConfPath, CPATH_MAX_LEN, "%s", c_AltConfPath);
	} else {
		// Check if ~/.config/kbd-backlight.conf exists
		snprintf(
			c_FinalConfPath, CPATH_MAX_LEN, "%s/%s", pw.pw_dir, CHOME_PATH);
		if (! fileExists(c_FinalConfPath)) {
			// If ~/.config/kbd-backlight.conf doesnt exist, fallback to
			// DEFAULT_CONF
			memset(c_FinalConfPath, '\0', CPATH_MAX_LEN + 1);
			snprintf(c_FinalConfPath, CPATH_MAX_LEN, DEFAULT_CONF);
		}
	}

	char c_ProfileRef[REF_MAX_LEN + 1];
	memset(c_ProfileRef, '\0', REF_MAX_LEN + 1);

	strcpy(c_ProfileRef, REF_PREFIX);
	strncat(c_ProfileRef, c_Pname, PNAME_MAX_LEN);

	Segment_Conf *conf = mkfullconf(c_FinalConfPath, c_ProfileRef);
	if (conf == nullptr) {
		free(conf);
		exit(EXIT_FAILURE);
	}

	char hid_dev[8] = {0};
	findDevice(hid_dev);

	writeConfig(hid_dev, conf);
	printf("%s[✔]%s Loaded profile: %s\n", GRN, RST, c_Pname);

	free(conf);

	exit(EXIT_SUCCESS);
}
