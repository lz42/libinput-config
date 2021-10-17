#include <errno.h>
#include <locale.h>
#include <string.h>

#include "config.h"
#include "keyfile.h"
#include "override.h"

#define cmp(a, b) strcmp(a, b) == 0
#define key(a) cmp(pair.key, a)
#define value(a) cmp(pair.value, a)

#define print(...)\
	fprintf(stderr, "libinput-config: ");\
	fprintf(stderr, __VA_ARGS__);\
	fprintf(stderr, "\n");\

#define invalid_key() print("warning: invalid setting key")
#define invalid_value() print("warning: invalid setting value")

struct libinput_config libinput_config = {
	.configured = false,
	
	.tap_configured = false,
	.tap_button_map_configured = false,
	.drag_configured = false,
	.drag_lock_configured = false,
	.accel_speed_configured = false,
	.accel_profile_configured = false,
	.natural_scroll_configured = false,
	.left_handed_configured = false,
	.click_method_configured = false,
	.middle_emulation_configured = false,
	.scroll_method_configured = false,
	.scroll_button_configured = false,
	.dwt_configured = false,
	
	.scroll_factor = 1,
	.discrete_scroll_factor = 1,
	.speed = 1
};

static bool parse_number(const char *string, double *number) {
	char *dummy = NULL;
	
	*number = strtod(string, &dummy);
	
	if (dummy[0] != '\0' || errno == EINVAL || errno == ERANGE) {
		errno = 0;
		
		return false;
	}
	
	return true;
}

void libinput_config_init(void) {
	print("initializing");
	
	if (libinput_config.configured) {
		print("already initialized");
		
		return;
	}
	libinput_config.configured = true;
	
	FILE *file = fopen("/etc/libinput.conf", "r");
	
	if (file == NULL) {
		print("couldn't read the config file");
		
		return;
	}
	
	// We need to switch locales to correctly parse numbers
	
	locale_t
		initial_locale = newlocale(LC_NUMERIC_MASK, "", NULL),
		c_locale = newlocale(LC_NUMERIC_MASK, "C", NULL);
	
	uselocale(c_locale);
	
	while (true) {
		struct libinput_keyfile_pair pair =
			libinput_keyfile_get_pair(file);
		
		if (pair.key == NULL || pair.value == NULL) {
			break;
		}
		
		print("option '%s' is '%s'", pair.key, pair.value);
		
		if (key("tap")) {
			libinput_config.tap_configured = true;
			
			if (value("disabled")) {
				libinput_config.tap = LIBINPUT_CONFIG_TAP_DISABLED;
			} else if (value("enabled")) {
				libinput_config.tap = LIBINPUT_CONFIG_TAP_ENABLED;
			} else {
				invalid_value();
				
				libinput_config.tap_configured = false;
			}
		} else if (key("tap-button-map")) {
			libinput_config.tap_button_map_configured = true;
			
			if (value("lrm")) {
				libinput_config.tap_button_map = LIBINPUT_CONFIG_TAP_MAP_LRM;
			} else if (value("lmr")) {
				libinput_config.tap_button_map = LIBINPUT_CONFIG_TAP_MAP_LMR;
			} else {
				invalid_value();
				
				libinput_config.tap_button_map_configured = false;
			}
		} else if (key("drag")) {
			libinput_config.drag_configured = true;
			
			if (value("disabled")) {
				libinput_config.drag = LIBINPUT_CONFIG_DRAG_DISABLED;
			} else if (value("enabled")) {
				libinput_config.drag = LIBINPUT_CONFIG_DRAG_ENABLED;
			} else {
				invalid_value();
				
				libinput_config.drag_configured = false;
			}
		} else if (key("drag-lock")) {
			libinput_config.drag_lock_configured = true;
			
			if (value("disabled")) {
				libinput_config.drag_lock = LIBINPUT_CONFIG_DRAG_LOCK_DISABLED;
			} else if (value("enabled")) {
				libinput_config.drag_lock = LIBINPUT_CONFIG_DRAG_LOCK_ENABLED;
			} else {
				invalid_value();
				
				libinput_config.drag_lock_configured = false;
			}
		} else if (key("accel-speed")) {
			libinput_config.accel_speed_configured = true;
			
			bool success = parse_number(
				pair.value,
				&libinput_config.accel_speed
			);
			
			if (!success) {
				invalid_value();
				
				libinput_config.accel_speed_configured = false;
			}
		} else if (key("accel-profile")) {
			libinput_config.accel_profile_configured = true;
			
			if (value("none")) {
				libinput_config.accel_profile = LIBINPUT_CONFIG_ACCEL_PROFILE_NONE;
			} else if (value("flat")) {
				libinput_config.accel_profile = LIBINPUT_CONFIG_ACCEL_PROFILE_FLAT;
			} else if (value("adaptive")) {
				libinput_config.accel_profile = LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE;
			} else {
				invalid_value();
				
				libinput_config.accel_profile_configured = false;
			}
		} else if (key("natural-scroll")) {
			libinput_config.natural_scroll_configured = true;
			
			if (value("disabled")) {
				libinput_config.natural_scroll = false;
			} else if (value("enabled")) {
				libinput_config.natural_scroll = true;
			} else {
				invalid_value();
				
				libinput_config.natural_scroll_configured = false;
			}
		} else if (key("left-handed")) {
			libinput_config.left_handed_configured = true;
			
			if (value("disabled")) {
				libinput_config.left_handed = false;
			} else if (value("enabled")) {
				libinput_config.left_handed = true;
			} else {
				invalid_value();
				
				libinput_config.left_handed_configured = false;
			}
		} else if (key("click-method")) {
			libinput_config.click_method_configured = true;
			
			if (value("none")) {
				libinput_config.click_method = LIBINPUT_CONFIG_CLICK_METHOD_NONE;
			} else if (value("button-areas")) {
				libinput_config.click_method = LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS;
			} else if (value("clickfinger")) {
				libinput_config.click_method = LIBINPUT_CONFIG_CLICK_METHOD_CLICKFINGER;
			} else {
				invalid_value();
				
				libinput_config.click_method_configured = false;
			}
		} else if (key("middle-emulation")) {
			libinput_config.middle_emulation_configured = true;
			
			if (value("disabled")) {
				libinput_config.middle_emulation = LIBINPUT_CONFIG_MIDDLE_EMULATION_DISABLED;
			} else if (value("enabled")) {
				libinput_config.middle_emulation = LIBINPUT_CONFIG_MIDDLE_EMULATION_ENABLED;
			} else {
				invalid_value();
				
				libinput_config.middle_emulation_configured = false;
			}
		} else if (key("scroll-method")) {
			libinput_config.scroll_method_configured = true;
			
			if (value("none")) {
				libinput_config.scroll_method = LIBINPUT_CONFIG_SCROLL_NO_SCROLL;
			} else if (value("two-fingers")) {
				libinput_config.scroll_method = LIBINPUT_CONFIG_SCROLL_2FG;
			} else if (value("edge")) {
				libinput_config.scroll_method = LIBINPUT_CONFIG_SCROLL_EDGE;
			} else if (value("on-button-down")) {
				libinput_config.scroll_method = LIBINPUT_CONFIG_SCROLL_ON_BUTTON_DOWN;
			} else {
				invalid_value();
				
				libinput_config.scroll_method_configured = false;
			}
		} else if (key("scroll-button")) {
			libinput_config.scroll_button_configured = true;
			
			double val = 0;
			
			bool success = parse_number(pair.value, &val);
			
			if (!success || val > UINT32_MAX || val < 0) {
				invalid_value();
				
				libinput_config.scroll_button_configured = false;
			} else {
				libinput_config.scroll_button = (uint32_t) val;
			}
		} else if (key("dwt")) {
			libinput_config.dwt_configured = true;
			
			if (value("disabled")) {
				libinput_config.dwt = LIBINPUT_CONFIG_DWT_DISABLED;
			} else if (value("enabled")) {
				libinput_config.dwt = LIBINPUT_CONFIG_DWT_ENABLED;
			} else {
				invalid_value();
				
				libinput_config.dwt_configured = false;
			}
		} else if (key("scroll-factor")) {
			bool success = parse_number(
				pair.value,
				&libinput_config.scroll_factor
			);
			
			if (!success) {
				invalid_value();
				
				libinput_config.scroll_factor = 1;
			}
		} else if (key("discrete-scroll-factor")) {
			bool success = parse_number(
				pair.value,
				&libinput_config.discrete_scroll_factor
			);
			
			if (!success) {
				invalid_value();
				
				libinput_config.discrete_scroll_factor = 1;
			}
		} else if (key("speed")) {
			bool success = parse_number(pair.value, &libinput_config.speed);
			
			if (!success) {
				invalid_value();
				
				libinput_config.speed = 1;
			}
		} else {
			invalid_key();
		}
		
		free(pair.key);
		free(pair.value);
	}
	
	uselocale(initial_locale);
	
	print("initialized");
}

void libinput_config_device(struct libinput_device *device) {
	print("configuring device '%s'", libinput_device_get_name(device));
	
	if (libinput_config.tap_configured) {
		libinput_real.tap_set_enabled(
			device,
			libinput_config.tap
		);
	}
	
	if (libinput_config.tap_button_map_configured) {
		libinput_real.tap_set_button_map(
			device,
			libinput_config.tap_button_map
		);
	}
	
	if (libinput_config.drag_configured) {
		libinput_real.tap_set_drag_enabled(
			device,
			libinput_config.drag
		);
	}
	
	if (libinput_config.drag_lock_configured) {
		libinput_real.tap_set_drag_lock_enabled(
			device,
			libinput_config.drag_lock
		);
	}
	
	if (libinput_config.accel_speed_configured) {
		libinput_real.accel_set_speed(
			device,
			libinput_config.accel_speed
		);
	}
	
	if (libinput_config.accel_profile_configured) {
		libinput_real.accel_set_profile(
			device,
			libinput_config.accel_profile
		);
	}
	
	if (libinput_config.natural_scroll_configured) {
		libinput_real.scroll_set_natural_scroll_enabled(
			device,
			libinput_config.natural_scroll
		);
	}
	
	if (libinput_config.left_handed_configured) {
		libinput_real.left_handed_set(
			device,
			libinput_config.left_handed
		);
	}
	
	if (libinput_config.click_method_configured) {
		libinput_real.click_set_method(
			device,
			libinput_config.click_method
		);
	}
	
	if (libinput_config.middle_emulation_configured) {
		libinput_real.middle_emulation_set_enabled(
			device,
			libinput_config.middle_emulation
		);
	}
	
	if (libinput_config.scroll_method_configured) {
		libinput_real.scroll_set_method(
			device,
			libinput_config.scroll_method
		);
	}
	
	if (libinput_config.scroll_button_configured) {
		libinput_real.scroll_set_button(
			device,
			libinput_config.scroll_button
		);
	}
	
	if (libinput_config.dwt_configured) {
		libinput_real.dwt_set_enabled(
			device,
			libinput_config.dwt
		);
	}
}
