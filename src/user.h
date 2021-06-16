#pragma once
#include <grp.h>
#include <pwd.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

struct passwd getUserPw() {
	return *getpwuid(getuid());
}

bool isUserRoot() {
	if (getuid() == 0)
		return 1;
	return 0;
}

bool isUserInGroup(struct passwd* pw, const char *grp) {
	int ngroups = 0;
	getgrouplist(pw->pw_name, pw->pw_gid, NULL, &ngroups);

	gid_t groups[ngroups];
	getgrouplist(pw->pw_name, pw->pw_gid, groups, &ngroups);

	for (int i = 0; i < ngroups; i++) {
		struct group *gr = getgrgid(groups[i]);
		if (gr != NULL && strcmp(grp, gr->gr_name) == 0) {
			return 1;
		}
	}
	return 0;
}