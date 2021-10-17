#define _GNU_SOURCE

#include <dlfcn.h>

#include "override.h"

struct libinput_real libinput_real = {
	.tap_set_enabled = NULL,
	.tap_set_button_map = NULL,
	.tap_set_drag_enabled = NULL,
	.tap_set_drag_lock_enabled = NULL,
	.accel_set_speed = NULL,
	.accel_set_profile = NULL,
	.scroll_set_natural_scroll_enabled = NULL,
	.left_handed_set = NULL,
	.click_set_method = NULL,
	.middle_emulation_set_enabled = NULL,
	.scroll_set_method = NULL,
	.scroll_set_button = NULL,
	.dwt_set_enabled = NULL
};

void libinput_real_init(void) {
	libinput_real = (struct libinput_real) {
		.tap_set_enabled =
			dlsym(RTLD_NEXT, "libinput_device_config_tap_set_enabled"),
		.tap_set_button_map =
			dlsym(RTLD_NEXT, "libinput_device_config_tap_set_button_map"),
		.tap_set_drag_enabled =
			dlsym(RTLD_NEXT, "libinput_device_config_tap_set_drag_enabled"),
		.tap_set_drag_lock_enabled =
			dlsym(RTLD_NEXT, "libinput_device_config_tap_set_drag_lock_enabled"),
		.accel_set_speed =
			dlsym(RTLD_NEXT, "libinput_device_config_accel_set_speed"),
		.accel_set_profile =
			dlsym(RTLD_NEXT, "libinput_device_config_accel_set_profile"),
		.scroll_set_natural_scroll_enabled =
			dlsym(RTLD_NEXT, "libinput_device_config_scroll_set_natural_scroll_enabled"),
		.left_handed_set =
			dlsym(RTLD_NEXT, "libinput_device_config_left_handed_set"),
		.click_set_method =
			dlsym(RTLD_NEXT, "libinput_device_config_click_set_method"),
		.middle_emulation_set_enabled =
			dlsym(RTLD_NEXT, "libinput_device_config_middle_emulation_set_enabled"),
		.scroll_set_method =
			dlsym(RTLD_NEXT, "libinput_device_config_scroll_set_method"),
		.scroll_set_button =
			dlsym(RTLD_NEXT, "libinput_device_config_scroll_set_button"),
		.dwt_set_enabled =
			dlsym(RTLD_NEXT, "libinput_device_config_dwt_set_enabled")
	};
}
