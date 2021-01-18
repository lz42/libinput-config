#define _GNU_SOURCE

#include <dlfcn.h>

#include "config.h"

typedef struct libinput *(*udev_create_context_t)(
	const struct libinput_interface *,
	void *,
	struct udev *
);
typedef struct libinput *(*path_create_context_t)(
	const struct libinput_interface *,
	void *
);

typedef struct libinput_event *(*get_event_t)(struct libinput *);

typedef double (*get_axis_value_t)(
	struct libinput_event_pointer *,
	enum libinput_pointer_axis
);

typedef double (*get_dx_t)(struct libinput_event_pointer *);

struct libinput *libinput_udev_create_context(
	const struct libinput_interface *interface,
	void *user_data,
	struct udev *udev
) {
	udev_create_context_t udev_create_context =
		dlsym(RTLD_NEXT, "libinput_udev_create_context");
	
	libinput_config_init();
	
	return udev_create_context(interface, user_data, udev);
}
struct libinput *libinput_path_create_context(
	const struct libinput_interface *interface,
	void *user_data
) {
	path_create_context_t path_create_context =
		dlsym(RTLD_NEXT, "libinput_path_create_context");
	
	libinput_config_init();
	
	return path_create_context(interface, user_data);
}

struct libinput_event *libinput_get_event(struct libinput *libinput) {
	get_event_t get_event = dlsym(RTLD_NEXT, "libinput_get_event");
	
	struct libinput_event *event = get_event(libinput);
	
	if (event == NULL) {
		return NULL;
	}
	
	if (libinput_event_get_type(event) == LIBINPUT_EVENT_DEVICE_ADDED) {
		libinput_config_device(libinput_event_get_device(event));
	}
	
	return event;
}

double libinput_event_pointer_get_axis_value (
	struct libinput_event_pointer *event,
	enum libinput_pointer_axis axis
) {
	get_axis_value_t get_axis_value =
		dlsym(RTLD_NEXT, "libinput_event_pointer_get_axis_value");
	
	return
		get_axis_value(event, axis) *
		libinput_config.scroll_factor;
}
double libinput_event_pointer_get_axis_value_discrete (
	struct libinput_event_pointer *event,
	enum libinput_pointer_axis axis
) {
	get_axis_value_t get_axis_value_discrete =
		dlsym(RTLD_NEXT, "libinput_event_pointer_get_axis_value_discrete");
	
	return
		get_axis_value_discrete(event, axis) *
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
