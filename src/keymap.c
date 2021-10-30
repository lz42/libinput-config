#include <stdlib.h>

#include "keymap.h"

static const uint32_t KEY_CODE_COUNT = 768;

uint32_t *libinput_keymap = NULL;

void libinput_keymap_init(void) {
	libinput_keymap = calloc(KEY_CODE_COUNT, sizeof(uint32_t));
}

bool libinput_keymap_set(uint32_t source, uint32_t destination) {
	if (source >= KEY_CODE_COUNT || destination >= KEY_CODE_COUNT) {
		return false;
	}
	
	libinput_keymap[source] = destination;
	
	return true;
}

uint32_t libinput_keymap_transform(uint32_t source) {
	if (source >= KEY_CODE_COUNT) {
		return source;
	}
	
	uint32_t destination = libinput_keymap[source];
	
	if (destination != 0) {
		return destination;
	}
	
	return source;
}
