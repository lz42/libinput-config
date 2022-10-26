#ifndef OVERRIDE_H
#define OVERRIDE_H

#include <libinput.h>

#define config_function(name, type)\
	enum libinput_config_status (*name)(\
		struct libinput_device *,\
		type\
	)

struct libinput_real {
	config_function(tap_set_enabled, elc(tap_state));
	config_function(tap_set_button_map, elc(tap_button_map));
	config_function(tap_set_drag_enabled, elc(drag_state));
	config_function(tap_set_drag_lock_enabled, elc(drag_lock_state));
	config_function(accel_set_speed, double);
	config_function(accel_set_profile, elc(accel_profile));
	config_function(scroll_set_natural_scroll_enabled, int);
	config_function(left_handed_set, int);
	config_function(click_set_method, elc(click_method));
	config_function(middle_emulation_set_enabled, elc(middle_emulation_state));
	config_function(scroll_set_method, elc(scroll_method));
	config_function(scroll_set_button, uint32_t);
	config_function(dwt_set_enabled, elc(dwt_state));
};

extern struct libinput_real libinput_real;

void libinput_real_init(void);

#endif
