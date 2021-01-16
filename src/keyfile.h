#ifndef KEYFILE_H
#define KEYFILE_H

#include <stdio.h>

struct libinput_keyfile_pair {
	char *key, *value;
};

struct libinput_keyfile_pair libinput_keyfile_get_pair(FILE *);

#endif
