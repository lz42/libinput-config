#define _GNU_SOURCE

#include <dlfcn.h>

#include "config.h"
#include "workaround.h"

typedef double (*get_axis_value_t)(
	struct libinput_event_pointer *,
	enum libinput_pointer_axis
);

typedef double (*get_dx_t)(struct libinput_event_pointer *);

double libinput_event_pointer_get_axis_value(
	struct libinput_event_pointer *event,
	enum libinput_pointer_axis axis
) {
	get_axis_value_t get_axis_value =
		dlsym(RTLD_NEXT, "libinput_event_pointer_get_axis_value");
	
	return
		get_axis_value(event, axis) *
		libinput_config.scroll_factor;
}
double libinput_event_pointer_get_axis_value_discrete(
	struct libinput_event_pointer *event,
	enum libinput_pointer_axis axis
) {
	get_axis_value_t get_axis_value_discrete =
		dlsym(RTLD_NEXT, "libinput_event_pointer_get_axis_value_discrete");
	
	return
		get_axis_value_discrete(event, axis) *
		libinput_config.discrete_scroll_factor;
}

double libinput_event_pointer_get_scroll_value(
	struct libinput_event_pointer *event,
	enum libinput_pointer_axis axis
) {
	get_axis_value_t get_scroll_value =
		dlsym(RTLD_NEXT, "libinput_event_pointer_get_scroll_value");
	
	return
		get_scroll_value(event, axis) *
		libinput_config.scroll_factor;
}
double libinput_event_pointer_get_scroll_value_v120(
	struct libinput_event_pointer *event,
	enum libinput_pointer_axis axis
) {
	get_axis_value_t get_scroll_value_v120 =
		dlsym(RTLD_NEXT, "libinput_event_pointer_get_scroll_value_v120");
	
	return
		get_scroll_value_v120(event, axis) *
		libinput_config.discrete_scroll_factor;
}

double libinput_event_pointer_get_dx(
	struct libinput_event_pointer *event
) {
	get_dx_t get_dx = dlsym(RTLD_NEXT, "libinput_event_pointer_get_dx");
	
	return get_dx(event) * libinput_config.speed;
}
double libinput_event_pointer_get_dy(
	struct libinput_event_pointer *event
) {
	get_dx_t get_dy = dlsym(RTLD_NEXT, "libinput_event_pointer_get_dy");
	
	return get_dy(event) * libinput_config.speed;
}

double libinput_event_pointer_get_dx_unaccelerated(
	struct libinput_event_pointer *event
) {
	get_dx_t get_dx_unaccelerated =
		dlsym(RTLD_NEXT, "libinput_event_pointer_get_dx_unaccelerated");
	
	return get_dx_unaccelerated(event) * libinput_config.speed;
}
double libinput_event_pointer_get_dy_unaccelerated(
	struct libinput_event_pointer *event
) {
	get_dx_t get_dy_unaccelerated =
		dlsym(RTLD_NEXT, "libinput_event_pointer_get_dy_unaccelerated");
	
	return get_dy_unaccelerated(event) * libinput_config.speed;
}
