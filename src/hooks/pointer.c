#define _GNU_SOURCE

#include <dlfcn.h>

#include "config.h"

typedef double (*hooked_t)(struct libinput_event_pointer *);

typedef double (*hooked_axis_t)(
	struct libinput_event_pointer *,
	enum libinput_pointer_axis
);

#define hook_event(prop, config)\
	double libinput_event_pointer_get_##prop(\
		struct libinput_event_pointer *event\
	) {\
		hooked_t hooked = hook("libinput_event_pointer_get_" stringify(prop));\
		\
		return hooked(event) * libinput_config.config;\
	}

#define hook_axis_event(prop, config)\
	double libinput_event_pointer_get_##prop(\
		struct libinput_event_pointer *event,\
		enum libinput_pointer_axis axis\
	) {\
		hooked_axis_t hooked =\
			hook("libinput_event_pointer_get_" stringify(prop));\
		\
		if (axis == LIBINPUT_POINTER_AXIS_SCROLL_HORIZONTAL) {\
			return\
				hooked(event, axis) *\
				libinput_config.config##_x;\
		} else {\
			return\
				hooked(event, axis) *\
				libinput_config.config##_y;\
		}\
	}

hook_event(dx, speed_x);
hook_event(dy, speed_y);
hook_event(dx_unaccelerated, speed_x);
hook_event(dy_unaccelerated, speed_y);

hook_axis_event(axis_value, scroll_factor);
hook_axis_event(axis_value_discrete, discrete_scroll_factor);
hook_axis_event(scroll_value, scroll_factor);
hook_axis_event(scroll_value_v120, discrete_scroll_factor);
