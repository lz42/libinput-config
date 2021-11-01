#include <stdlib.h>
#include <string.h>

#include "keyfile.h"

struct libinput_keyfile_pair libinput_keyfile_get_pair(FILE *file) {
	struct libinput_keyfile_pair pair = {
		.key = NULL,
		.value = NULL
	};
	
	char *line = NULL;
	size_t size = 0;
	
	char *delim = NULL;
	
	do {
		free(line);
		line = NULL;
		
		if (getline(&line, &size, file) < 0) {
			return pair;
		}
		
		delim = strchr(line, '=');
	} while (delim == NULL);
	
	char *end = strchr(line, '\n');
	
	if (end == NULL) {
		end = line + strlen(line);
	}
	
	*delim = '\0';
	*end = '\0';
	
	pair.key = line;
	pair.value = delim + 1;
	
	return pair;
}
