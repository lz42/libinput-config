#ifndef CONFIG_H
#define CONFIG_H

#include <libinput.h>
#include <stdbool.h>

#define config_option(type, name)\
	bool name##_configured;\
	type name;

struct libinput_config {
	bool configured;
	
	bool override_compositor;
	
	config_option(enum libinput_config_tap_state, tap)
	config_option(enum libinput_config_tap_button_map, tap_button_map)
	config_option(enum libinput_config_drag_state, drag)
	config_option(enum libinput_config_drag_lock_state, drag_lock)
	config_option(double, accel_speed)
	config_option(enum libinput_config_accel_profile, accel_profile)
	config_option(bool, natural_scroll)
	config_option(bool, left_handed)
	config_option(enum libinput_config_click_method, click_method)
	config_option(enum libinput_config_middle_emulation_state, middle_emulation)
	config_option(enum libinput_config_scroll_method, scroll_method)
	config_option(uint32_t, scroll_button)
	config_option(enum libinput_config_dwt_state, dwt)
	
	double scroll_factor;
	double discrete_scroll_factor;
	double speed;
};

extern struct libinput_config libinput_config;

void libinput_config_init(void);
void libinput_config_device(struct libinput_device *);

#endif
