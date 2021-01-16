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
