#include <stdlib.h>
#include <string.h>

#include "keyfile.h"

static char *string_slice(char *str, size_t start, size_t end) {
	size_t size = end - start;
	
	char *output = malloc(size * sizeof(char));
	
	if (output == NULL) {
		return NULL;
	}
	
	strncpy(output, str + start, size);
	
	output[size] = '\0';
	
	return output;
}

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
	
	size_t delim_pos = delim - line;
	size_t end_pos = end - line;
	
	pair.key = string_slice(line, 0, delim_pos);
	pair.value = string_slice(line, delim_pos + 1, end_pos);
	
	free(line);
	
	return pair;
}
