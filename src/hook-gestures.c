#define _GNU_SOURCE

#include <dlfcn.h>

#include "config.h"
#include "workaround.h"

typedef double (*hooked_t)(struct libinput_event_gesture *);

#define hook_event(prop, config)\
	double libinput_event_gesture_get_##prop(\
		struct libinput_event_gesture *event\
	) {\
		hooked_t hooked = hook("libinput_event_gesture_get_" stringify(prop));\
		\
		return hooked(event) * libinput_config.config;\
	}

hook_event(dx, gesture_speed_x);
hook_event(dy, gesture_speed_y);
hook_event(dx_unaccelerated, gesture_speed_x);
hook_event(dy_unaccelerated, gesture_speed_y);
