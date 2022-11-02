#define _GNU_SOURCE

#include <dlfcn.h>

#include "config.h"

#define config_option(name, function)\
	.name##_configured = false,\
	.function = NULL

#define load_function(name)\
	libinput_config.name = dlsym(RTLD_NEXT,\
		"libinput_device_config_" stringify(name)\
	)

#define replace_function(name, config_name, type)\
	elc(status) libinput_device_config_##name(\
		struct libinput_device *a,\
		type b\
	){\
		if (\
			libinput_config.override_compositor &&\
			libinput_config.config_name##_configured\
		) {\
			return LIBINPUT_CONFIG_STATUS_SUCCESS;\
		} else {\
			return libinput_config.name(a, b);\
		}\
	}

#define apply_config(name, function_name)\
	if (libinput_config.name##_configured) {\
		libinput_config.function_name(\
			device,\
			libinput_config.name\
		);\
	}

struct libinput_config libinput_config = {
	.configured = false,
	
	.override_compositor = false,
	
	config_option(tap, tap_set_enabled),
	config_option(tap_button_map, tap_set_button_map),
	config_option(drag, tap_set_drag_enabled),
	config_option(drag_lock, tap_set_drag_lock_enabled),
	config_option(scroll_button_lock, scroll_set_button_lock_enabled),
	config_option(accel_speed, accel_set_speed),
	config_option(accel_profile, accel_set_profile),
	config_option(natural_scroll, scroll_set_natural_scroll_enabled),
	config_option(left_handed, left_handed_set),
	config_option(click_method, click_set_method),
	config_option(middle_emulation, middle_emulation_set_enabled),
	config_option(scroll_method, scroll_set_method),
	config_option(scroll_button, scroll_set_button),
	config_option(dwt, dwt_set_enabled),
	config_option(dwtp, dwtp_set_enabled),
	
	.scroll_factor_x = 1,
	.scroll_factor_y = 1,
	
	.discrete_scroll_factor_x = 1,
	.discrete_scroll_factor_y = 1,
	
	.speed_x = 1,
	.speed_y = 1,
	
	.gesture_speed_x = 1,
	.gesture_speed_y = 1
};

void libinput_real_init(void) {
	load_function(tap_set_enabled);
	load_function(tap_set_button_map);
	load_function(tap_set_drag_enabled);
	load_function(tap_set_drag_lock_enabled);
	load_function(scroll_set_button_lock_enabled);
	load_function(accel_set_speed);
	load_function(accel_set_profile);
	load_function(scroll_set_natural_scroll_enabled);
	load_function(left_handed_set);
	load_function(click_set_method);
	load_function(middle_emulation_set_enabled);
	load_function(scroll_set_method);
	load_function(scroll_set_button);
	load_function(dwt_set_enabled);
	load_function(dwtp_set_enabled);
}

replace_function(tap_set_enabled, tap, elc(tap_state));
replace_function(tap_set_button_map, tap_button_map, elc(tap_button_map));
replace_function(tap_set_drag_enabled, drag, elc(drag_state));
replace_function(tap_set_drag_lock_enabled, drag_lock, elc(drag_lock_state));
replace_function(scroll_set_button_lock_enabled, scroll_button_lock, elc(scroll_button_lock_state));
replace_function(accel_set_speed, accel_speed, double);
replace_function(accel_set_profile, accel_profile, elc(accel_profile));
replace_function(scroll_set_natural_scroll_enabled, natural_scroll, int);
replace_function(left_handed_set, left_handed, int);
replace_function(click_set_method, click_method, elc(click_method));
replace_function(middle_emulation_set_enabled, middle_emulation, elc(middle_emulation_state));
replace_function(scroll_set_method, scroll_method, elc(scroll_method));
replace_function(scroll_set_button, scroll_button, uint32_t);
replace_function(dwt_set_enabled, dwt, elc(dwt_state));
replace_function(dwtp_set_enabled, dwtp, elc(dwtp_state));

void libinput_config_device(struct libinput_device *device) {
	print("configuring device '%s'", libinput_device_get_name(device));
	
	apply_config(tap, tap_set_enabled);
	apply_config(tap_button_map, tap_set_button_map);
	apply_config(drag, tap_set_drag_enabled);
	apply_config(drag_lock, tap_set_drag_lock_enabled);
	apply_config(scroll_button_lock, scroll_set_button_lock_enabled);
	apply_config(accel_speed, accel_set_speed);
	apply_config(accel_profile, accel_set_profile);
	apply_config(natural_scroll, scroll_set_natural_scroll_enabled);
	apply_config(left_handed, left_handed_set);
	apply_config(click_method, click_set_method);
	apply_config(middle_emulation, middle_emulation_set_enabled);
	apply_config(scroll_method, scroll_set_method);
	apply_config(scroll_button, scroll_set_button);
	apply_config(dwt, dwt_set_enabled);
	apply_config(dwtp, dwtp_set_enabled);
}
