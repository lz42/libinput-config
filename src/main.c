#include <errno.h>
#include <locale.h>
#include <string.h>

#include "config.h"
#include "keyfile.h"
#include "keymap.h"
#include "override.h"
#include "workaround.h"

#define cmp(a, b) strcmp(a, b) == 0

#define keys_start if (false) {}
#define keys_end else {invalid_value();}

#define values_start keys_start
#define values_end(config) else {\
	invalid_value();\
	\
	libinput_config.config = false;\
}

#define fancy_values_start(config)\
	libinput_config.config##_configured = true;\
	values_start
#define fancy_values_end(config) values_end(config##_configured)

#define key(a) else if (cmp(pair.key, a))
#define value(a) if (cmp(pair.value, a))

#define print(...)\
	fprintf(stderr, "libinput-config: ");\
	fprintf(stderr, __VA_ARGS__);\
	fprintf(stderr, "\n");\

#define invalid_key() print("warning: invalid setting key")
#define invalid_value() print("warning: invalid setting value")

#define keyword_value(name, config, prop)\
	value(name) {\
		libinput_config.config = prop;\
	}

#define binary_value(config)\
	keyword_value("disabled", config, false)\
	keyword_value("enabled", config, true)
#define fancy_binary_value(config, CONFIG)\
	keyword_value("disabled", config, LIBINPUT_CONFIG_##CONFIG##_DISABLED)\
	keyword_value("enabled", config, LIBINPUT_CONFIG_##CONFIG##_ENABLED)

#define binary_preset(name, config)\
	key(name) {\
		fancy_values_start(config)\
		\
		binary_value(config)\
		\
		fancy_values_end(config)\
	}
#define fancy_binary_preset(name, config, CONFIG)\
	key(name) {\
		fancy_values_start(config)\
		\
		fancy_binary_value(config, CONFIG)\
		\
		fancy_values_end(config)\
	}

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
	
	.scroll_factor_x = 1,
	.scroll_factor_y = 1,
	
	.discrete_scroll_factor_x = 1,
	.discrete_scroll_factor_y = 1,
	
	.speed_x = 1,
	.speed_y = 1,
	
	.gesture_speed_x = 1,
	.gesture_speed_y = 1
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
	
	print("replacing the device config functions");
	libinput_real_init();
	
	print("initializing the key mapper");
	libinput_keymap_init();
	
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
		
		keys_start
		
		key("override-compositor") {
			values_start
			
			binary_value(override_compositor)
			
			values_end(override_compositor)
		}
		
		binary_preset("natural-scroll", natural_scroll)
		binary_preset("left-handed", left_handed)
		
		fancy_binary_preset("tap", tap, TAP)
		fancy_binary_preset("drag", drag, DRAG)
		fancy_binary_preset("drag-lock", drag_lock, DRAG_LOCK)
		fancy_binary_preset("middle-emulation", middle_emulation, MIDDLE_EMULATION)
		fancy_binary_preset("dwt", dwt, DWT)
		
		key("tap-button-map") {
			fancy_values_start(tap_button_map)
			
			keyword_value("lrm", tap_button_map, LIBINPUT_CONFIG_TAP_MAP_LRM)
			keyword_value("lmr", tap_button_map, LIBINPUT_CONFIG_TAP_MAP_LMR)
			
			fancy_values_end(tap_button_map)
		}
		
		key("accel-profile") {
			fancy_values_start(accel_profile)
			
			keyword_value("none", accel_profile, LIBINPUT_CONFIG_ACCEL_PROFILE_NONE)
			keyword_value("flat", accel_profile, LIBINPUT_CONFIG_ACCEL_PROFILE_FLAT)
			keyword_value("adaptive", accel_profile, LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE)
			
			fancy_values_end(accel_profile)
		}
		
		key("click-method") {
			fancy_values_start(click_method)
			
			keyword_value("none", click_method, LIBINPUT_CONFIG_CLICK_METHOD_NONE)
			keyword_value("button-areas", click_method, LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS)
			keyword_value("clickfinger", click_method, LIBINPUT_CONFIG_CLICK_METHOD_CLICKFINGER)
			
			fancy_values_end(click_method)
		}
		
		key("scroll-method") {
			fancy_values_start(scroll_method)
			
			keyword_value("none", scroll_method, LIBINPUT_CONFIG_SCROLL_NO_SCROLL)
			keyword_value("two-fingers", scroll_method, LIBINPUT_CONFIG_SCROLL_2FG)
			keyword_value("edge", scroll_method, LIBINPUT_CONFIG_SCROLL_EDGE)
			keyword_value("on-button-down", scroll_method, LIBINPUT_CONFIG_SCROLL_ON_BUTTON_DOWN)
			
			fancy_values_end(scroll_method)
		}
		
		key("accel-speed") {
			libinput_config.accel_speed_configured = true;
			
			bool success = parse_number(
				pair.value,
				&libinput_config.accel_speed
			);
			
			if (!success) {
				invalid_value();
				
				libinput_config.accel_speed_configured = false;
			}
		}
		
		key("scroll-button") {
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
		}
		
		key("scroll-factor") {
			bool success = parse_number(
				pair.value,
				&libinput_config.scroll_factor_x
			);
			
			if (!success) {
				invalid_value();
				
				libinput_config.scroll_factor_x = 1;
				libinput_config.scroll_factor_y = 1;
			} else {
				libinput_config.scroll_factor_y =
					libinput_config.scroll_factor_x;
			}
		}
		
		key("scroll-factor-x") {
			bool success = parse_number(
				pair.value,
				&libinput_config.scroll_factor_x
			);
			
			if (!success) {
				invalid_value();
				
				libinput_config.scroll_factor_x = 1;
			}
		}
		
		key("scroll-factor-y") {
			bool success = parse_number(
				pair.value,
				&libinput_config.scroll_factor_y
			);
			
			if (!success) {
				invalid_value();
				
				libinput_config.scroll_factor_y = 1;
			}
		}
		
		key("discrete-scroll-factor") {
			bool success = parse_number(
				pair.value,
				&libinput_config.discrete_scroll_factor_x
			);
			
			if (!success) {
				invalid_value();
				
				libinput_config.discrete_scroll_factor_x = 1;
				libinput_config.discrete_scroll_factor_y = 1;
			} else {
				libinput_config.discrete_scroll_factor_y =
					libinput_config.discrete_scroll_factor_x;
			}
		}
		
		key("discrete-scroll-factor-x") {
			bool success = parse_number(
				pair.value,
				&libinput_config.discrete_scroll_factor_x
			);
			
			if (!success) {
				invalid_value();
				
				libinput_config.discrete_scroll_factor_x = 1;
			}
		}
		
		key("discrete-scroll-factor-y") {
			bool success = parse_number(
				pair.value,
				&libinput_config.discrete_scroll_factor_y
			);
			
			if (!success) {
				invalid_value();
				
				libinput_config.discrete_scroll_factor_y = 1;
			}
		}
		
		key("speed") {
			bool success = parse_number(pair.value, &libinput_config.speed_x);
			
			if (!success) {
				invalid_value();
				
				libinput_config.speed_x = 1;
				libinput_config.speed_y = 1;
			} else {
				libinput_config.speed_y =
					libinput_config.speed_x;
			}
		}
		
		key("speed-x") {
			bool success = parse_number(pair.value, &libinput_config.speed_x);
			
			if (!success) {
				invalid_value();
				
				libinput_config.speed_x = 1;
			}
		}
		
		key("speed-y") {
			bool success = parse_number(pair.value, &libinput_config.speed_y);
			
			if (!success) {
				invalid_value();
				
				libinput_config.speed_y = 1;
			}
		}
		
		key("gesture-speed") {
			bool success = parse_number(
				pair.value,
				&libinput_config.gesture_speed_x
			);
			
			if (!success) {
				invalid_value();
				
				libinput_config.gesture_speed_x = 1;
				libinput_config.gesture_speed_y = 1;
			} else {
				libinput_config.gesture_speed_y =
					libinput_config.gesture_speed_x;
			}
		}
		
		key("gesture-speed-x") {
			bool success = parse_number(
				pair.value,
				&libinput_config.gesture_speed_x
			);
			
			if (!success) {
				invalid_value();
				
				libinput_config.gesture_speed_x = 1;
			}
		}
		
		key("gesture-speed-y") {
			bool success = parse_number(
				pair.value,
				&libinput_config.gesture_speed_y
			);
			
			if (!success) {
				invalid_value();
				
				libinput_config.gesture_speed_y = 1;
			}
		}
		
		key("remap-key") {
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
		}
		
		keys_end
		
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
