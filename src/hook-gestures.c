#define _GNU_SOURCE

#include <dlfcn.h>

#include "config.h"
#include "workaround.h"

typedef double (*get_dx_t)(struct libinput_event_gesture *);

double libinput_event_gesture_get_dx(
	struct libinput_event_gesture *event
) {
	get_dx_t get_dx = dlsym(RTLD_NEXT, "libinput_event_gesture_get_dx");
	
	return get_dx(event) * libinput_config.gesture_speed_x;
}
double libinput_event_gesture_get_dy(
	struct libinput_event_gesture *event
) {
	get_dx_t get_dy = dlsym(RTLD_NEXT, "libinput_event_gesture_get_dy");
	
	return get_dy(event) * libinput_config.gesture_speed_y;
}

double libinput_event_gesture_get_dx_unaccelerated(
	struct libinput_event_gesture *event
) {
	get_dx_t get_dx_unaccelerated =
		dlsym(RTLD_NEXT, "libinput_event_gesture_get_dx_unaccelerated");
	
	return
		get_dx_unaccelerated(event) *
		libinput_config.gesture_speed_x;
}
double libinput_event_gesture_get_dy_unaccelerated(
	struct libinput_event_gesture *event
) {
	get_dx_t get_dy_unaccelerated =
		dlsym(RTLD_NEXT, "libinput_event_gesture_get_dy_unaccelerated");
	
	return
		get_dy_unaccelerated(event) *
		libinput_config.gesture_speed_y;
}
