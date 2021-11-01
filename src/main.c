#include <errno.h>
#include <locale.h>
#include <string.h>

#include "config.h"
#include "keyfile.h"
#include "keymap.h"
#include "override.h"
#include "workaround.h"

#define cmp(a, b) strcmp(a, b) == 0
#define key(a) cmp(pair.key, a)
#define value(a) cmp(pair.value, a)

#define print(...)\
	fprintf(stderr, "libinput-config: ");\
	fprintf(stderr, __VA_ARGS__);\
	fprintf(stderr, "\n");\

#define invalid_key() print("warning: invalid setting key")
#define invalid_value() print("warning: invalid setting value")

#define apply_config(name, function_name)\
	if (libinput_config.name##_configured) {\
		libinput_real.function_name(\
			device,\
			libinput_config.name\
		);\
	}

struct libinput_config libinput_config = {
	.configured = false,
	
	.override_compositor = false,
	
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
	.speed = 1,
	.gesture_speed = 1
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
	
	print("replacing the device config functions");
	libinput_real_init();
	
	print("initializing the key mapper");
	libinput_keymap_init();
	
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
		
		if (key("override-compositor")) {
			if (value("disabled")) {
				libinput_config.override_compositor = false;
			} else if (value("enabled")) {
				libinput_config.override_compositor = true;
			} else {
				invalid_value();
				
				libinput_config.override_compositor = false;
			}
		} else if (key("tap")) {
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
			
			bool success =
				parse_number(pair.value, &val) &&
				
				val >= 1 &&
				val <= UINT32_MAX;
			
			if (!success) {
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
		} else if (key("gesture-speed")) {
			bool success = parse_number(
				pair.value,
				&libinput_config.gesture_speed
			);
			
			if (!success) {
				invalid_value();
				
				libinput_config.gesture_speed = 1;
			}
		} else if (key("remap-key")) {
			char *delim = strchr(pair.value, ':');
			
			if (delim == NULL) {
				invalid_value();
			} else {
				*delim = '\0';
				
				char
					*source_str = pair.value,
					*destination_str = delim + 1;
				
				double
					source = 0,
					destination = 0;
				
				bool success =
					parse_number(source_str, &source) &&
					parse_number(destination_str, &destination) &&
					
					source >= 1 &&
					source <= UINT32_MAX &&
					
					destination >= 1 &&
					destination <= UINT32_MAX &&
					
					libinput_keymap_set((uint32_t) source, (uint32_t) destination);
				
				if (!success) {
					invalid_value();
				}
			}
		} else {
			invalid_key();
		}
		
		free(pair.key);
	}
	
	fclose(file);
	
	uselocale(initial_locale);
	
	print("initialized");
}

void libinput_config_device(struct libinput_device *device) {
	print("configuring device '%s'", libinput_device_get_name(device));
	
	apply_config(tap, tap_set_enabled);
	apply_config(tap_button_map, tap_set_button_map);
	apply_config(drag, tap_set_drag_enabled);
	apply_config(drag_lock, tap_set_drag_lock_enabled);
	apply_config(accel_speed, accel_set_speed);
	apply_config(accel_profile, accel_set_profile);
	apply_config(natural_scroll, scroll_set_natural_scroll_enabled);
	apply_config(left_handed, left_handed_set);
	apply_config(click_method, click_set_method);
	apply_config(middle_emulation, middle_emulation_set_enabled);
	apply_config(scroll_method, scroll_set_method);
	apply_config(scroll_button, scroll_set_button);
	apply_config(dwt, dwt_set_enabled);
}
