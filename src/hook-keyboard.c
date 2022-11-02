#define _GNU_SOURCE

#include <dlfcn.h>

#include "config.h"

typedef uint32_t (*get_key_t)(struct libinput_event_keyboard *);

uint32_t libinput_event_keyboard_get_key(
	struct libinput_event_keyboard *event
) {
	get_key_t get_key =
		hook("libinput_event_keyboard_get_key");
	
	return libinput_keymap_transform(get_key(event));
}
