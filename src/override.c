#define _GNU_SOURCE

#include <dlfcn.h>

#include "config.h"
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

enum libinput_config_status libinput_device_config_tap_set_enabled(
	struct libinput_device *device,
	enum libinput_config_tap_state enable
){
	if (
		libinput_config.override_compositor &&
		libinput_config.tap_configured
	) {
		return LIBINPUT_CONFIG_STATUS_SUCCESS;
	} else {
		return libinput_real.tap_set_enabled(device, enable);
	}
}
enum libinput_config_status libinput_device_config_tap_set_button_map(
	struct libinput_device *device,
	enum libinput_config_tap_button_map map
){
	if (
		libinput_config.override_compositor &&
		libinput_config.tap_button_map_configured
	) {
		return LIBINPUT_CONFIG_STATUS_SUCCESS;
	} else {
		return libinput_real.tap_set_button_map(device, map);
	}
}
enum libinput_config_status libinput_device_config_tap_set_drag_enabled(
	struct libinput_device *device,
	enum libinput_config_drag_state enable
){
	if (
		libinput_config.override_compositor &&
		libinput_config.drag_configured
	) {
		return LIBINPUT_CONFIG_STATUS_SUCCESS;
	} else {
		return libinput_real.tap_set_drag_enabled(device, enable);
	}
}
enum libinput_config_status libinput_device_config_tap_set_drag_lock_enabled(
	struct libinput_device *device,
	enum libinput_config_drag_lock_state enable
){
	if (
		libinput_config.override_compositor &&
		libinput_config.drag_lock_configured
	) {
		return LIBINPUT_CONFIG_STATUS_SUCCESS;
	} else {
		return libinput_real.tap_set_drag_lock_enabled(device, enable);
	}
}
enum libinput_config_status libinput_device_config_accel_set_speed(
	struct libinput_device *device,
	double speed
){
	if (
		libinput_config.override_compositor &&
		libinput_config.accel_speed_configured
	) {
		return LIBINPUT_CONFIG_STATUS_SUCCESS;
	} else {
		return libinput_real.accel_set_speed(device, speed);
	}
}
enum libinput_config_status libinput_device_config_accel_set_profile(
	struct libinput_device *device,
	enum libinput_config_accel_profile profile
){
	if (
		libinput_config.override_compositor &&
		libinput_config.accel_profile_configured
	) {
		return LIBINPUT_CONFIG_STATUS_SUCCESS;
	} else {
		return libinput_real.accel_set_profile(device, profile);
	}
}
enum libinput_config_status libinput_device_config_scroll_set_natural_scroll_enabled(
	struct libinput_device *device,
	int enable
){
	if (
		libinput_config.override_compositor &&
		libinput_config.natural_scroll_configured
	) {
		return LIBINPUT_CONFIG_STATUS_SUCCESS;
	} else {
		return libinput_real.scroll_set_natural_scroll_enabled(device, enable);
	}
}
enum libinput_config_status libinput_device_config_left_handed_set(
	struct libinput_device *device,
	int left_handed
){
	if (
		libinput_config.override_compositor &&
		libinput_config.left_handed_configured
	) {
		return LIBINPUT_CONFIG_STATUS_SUCCESS;
	} else {
		return libinput_real.left_handed_set(device, left_handed);
	}
}
enum libinput_config_status libinput_device_config_click_set_method(
	struct libinput_device *device,
	enum libinput_config_click_method method
){
	if (
		libinput_config.override_compositor &&
		libinput_config.click_method_configured
	) {
		return LIBINPUT_CONFIG_STATUS_SUCCESS;
	} else {
		return libinput_real.click_set_method(device, method);
	}
}
enum libinput_config_status libinput_device_config_middle_emulation_set_enabled(
	struct libinput_device *device,
	enum libinput_config_middle_emulation_state enable
){
	if (
		libinput_config.override_compositor &&
		libinput_config.middle_emulation_configured
	) {
		return LIBINPUT_CONFIG_STATUS_SUCCESS;
	} else {
		return libinput_real.middle_emulation_set_enabled(device, enable);
	}
}
enum libinput_config_status libinput_device_config_scroll_set_method(
	struct libinput_device *device,
	enum libinput_config_scroll_method method
){
	if (
		libinput_config.override_compositor &&
		libinput_config.scroll_method_configured
	) {
		return LIBINPUT_CONFIG_STATUS_SUCCESS;
	} else {
		return libinput_real.scroll_set_method(device, method);
	}
}
enum libinput_config_status libinput_device_config_scroll_set_button(
	struct libinput_device *device,
	uint32_t button
){
	if (
		libinput_config.override_compositor &&
		libinput_config.scroll_button_configured
	) {
		return LIBINPUT_CONFIG_STATUS_SUCCESS;
	} else {
		return libinput_real.scroll_set_button(device, button);
	}
}
enum libinput_config_status libinput_device_config_dwt_set_enabled(
	struct libinput_device *device,
	enum libinput_config_dwt_state enable
){
	if (
		libinput_config.override_compositor &&
		libinput_config.dwt_configured
	) {
		return LIBINPUT_CONFIG_STATUS_SUCCESS;
	} else {
		return libinput_real.dwt_set_enabled(device, enable);
	}
}
