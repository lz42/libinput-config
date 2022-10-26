#define _GNU_SOURCE

#include <dlfcn.h>

#include "config.h"
#include "override.h"
#include "workaround.h"

#define xstringify(a) #a
#define stringify(a) xstringify(a)

#define load_function(name)\
	.name = dlsym(RTLD_NEXT, "libinput_device_config_" stringify(name))

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
			return libinput_real.name(a, b);\
		}\
	}

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
		load_function(tap_set_enabled),
		load_function(tap_set_button_map),
		load_function(tap_set_drag_enabled),
		load_function(tap_set_drag_lock_enabled),
		load_function(accel_set_speed),
		load_function(accel_set_profile),
		load_function(scroll_set_natural_scroll_enabled),
		load_function(left_handed_set),
		load_function(click_set_method),
		load_function(middle_emulation_set_enabled),
		load_function(scroll_set_method),
		load_function(scroll_set_button),
		load_function(dwt_set_enabled)
	};
}

replace_function(tap_set_enabled, tap, elc(tap_state));
replace_function(tap_set_button_map, tap_button_map, elc(tap_button_map));
replace_function(tap_set_drag_enabled, drag, elc(drag_state));
replace_function(tap_set_drag_lock_enabled, drag_lock, elc(drag_lock_state));
replace_function(accel_set_speed, accel_speed, double);
replace_function(accel_set_profile, accel_profile, elc(accel_profile));
replace_function(scroll_set_natural_scroll_enabled, natural_scroll, int);
replace_function(left_handed_set, left_handed, int);
replace_function(click_set_method, click_method, elc(click_method));
replace_function(middle_emulation_set_enabled, middle_emulation, elc(middle_emulation_state));
replace_function(scroll_set_method, scroll_method, elc(scroll_method));
replace_function(scroll_set_button, scroll_button, uint32_t);
replace_function(dwt_set_enabled, dwt, elc(dwt_state));
