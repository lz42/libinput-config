#include <string.h>

#include "config.h"
#include "keyfile.h"

#define cmp(a, b) strcmp(a, b) == 0
#define key(a) cmp(pair.key, a)
#define value(a) cmp(pair.value, a)

#define print(...)\
	fprintf(stderr, "libinput-config: ");\
	fprintf(stderr, __VA_ARGS__);\
	fprintf(stderr, "\n");\

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
			}
		} else if (key("tap-button-map")) {
			libinput_config.tap_button_map_configured = true;
			
			if (value("lrm")) {
				libinput_config.tap_button_map = LIBINPUT_CONFIG_TAP_MAP_LRM;
			} else if (value("lmr")) {
				libinput_config.tap_button_map = LIBINPUT_CONFIG_TAP_MAP_LMR;
			}
		} else if (key("drag")) {
			libinput_config.drag_configured = true;
			
			if (value("disabled")) {
				libinput_config.drag = LIBINPUT_CONFIG_DRAG_DISABLED;
			} else if (value("enabled")) {
				libinput_config.drag = LIBINPUT_CONFIG_DRAG_ENABLED;
			}
		} else if (key("drag-lock")) {
			libinput_config.drag_lock_configured = true;
			
			if (value("disabled")) {
				libinput_config.drag_lock = LIBINPUT_CONFIG_DRAG_LOCK_DISABLED;
			} else if (value("enabled")) {
				libinput_config.drag_lock = LIBINPUT_CONFIG_DRAG_LOCK_ENABLED;
			}
		} else if (key("accel-speed")) {
			libinput_config.accel_speed_configured = true;
			
			libinput_config.accel_speed = atof(pair.value);
		} else if (key("accel-profile")) {
			libinput_config.accel_profile_configured = true;
			
			if (value("none")) {
				libinput_config.accel_profile = LIBINPUT_CONFIG_ACCEL_PROFILE_NONE;
			} else if (value("flat")) {
				libinput_config.accel_profile = LIBINPUT_CONFIG_ACCEL_PROFILE_FLAT;
			} else if (value("adaptive")) {
				libinput_config.accel_profile = LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE;
			}
		} else if (key("natural-scroll")) {
			libinput_config.natural_scroll_configured = true;
			
			if (value("disabled")) {
				libinput_config.natural_scroll = false;
			} else if (value("enabled")) {
				libinput_config.natural_scroll = true;
			}
		} else if (key("left-handed")) {
			libinput_config.left_handed_configured = true;
			
			if (value("disabled")) {
				libinput_config.left_handed = false;
			} else if (value("enabled")) {
				libinput_config.left_handed = true;
			}
		} else if (key("click-method")) {
			libinput_config.click_method_configured = true;
			
			if (value("none")) {
				libinput_config.click_method = LIBINPUT_CONFIG_CLICK_METHOD_NONE;
			} else if (value("button-areas")) {
				libinput_config.click_method = LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS;
			} else if (value("clickfinger")) {
				libinput_config.click_method = LIBINPUT_CONFIG_CLICK_METHOD_CLICKFINGER;
			}
		} else if (key("middle-emulation")) {
			libinput_config.middle_emulation_configured = true;
			
			if (value("disabled")) {
				libinput_config.middle_emulation = LIBINPUT_CONFIG_MIDDLE_EMULATION_DISABLED;
			} else if (value("enabled")) {
				libinput_config.middle_emulation = LIBINPUT_CONFIG_MIDDLE_EMULATION_ENABLED;
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
			}
		} else if (key("scroll-button")) {
			libinput_config.scroll_button_configured = true;
			
			unsigned long int val = strtoul(pair.value, NULL, 0);
			
			if (val <= UINT32_MAX) {
				libinput_config.scroll_button = (uint32_t)val;
			}
		} else if (key("dwt")) {
			libinput_config.dwt_configured = true;
			
			if (value("disabled")) {
				libinput_config.dwt = LIBINPUT_CONFIG_DWT_DISABLED;
			} else if (value("enabled")) {
				libinput_config.dwt = LIBINPUT_CONFIG_DWT_ENABLED;
			}
		} else if (key("scroll-factor")) {
			libinput_config.scroll_factor = atof(pair.value);
		} else if (key("discrete-scroll-factor")) {
			libinput_config.discrete_scroll_factor = atof(pair.value);
		} else if (key("speed")) {
			libinput_config.speed = atof(pair.value);
		}
		
		free(pair.key);
		free(pair.value);
	}
	
	print("initialized");
}

void libinput_config_device(struct libinput_device *device) {
	print("configuring device '%s'", libinput_device_get_name(device));
	
	if (libinput_config.tap_configured) {
		libinput_device_config_tap_set_enabled(
			device,
			libinput_config.tap
		);
	}
	
	if (libinput_config.tap_button_map_configured) {
		libinput_device_config_tap_set_button_map(
			device,
			libinput_config.tap_button_map
		);
	}
	
	if (libinput_config.drag_configured) {
		libinput_device_config_tap_set_drag_enabled(
			device,
			libinput_config.drag
		);
	}
	
	if (libinput_config.drag_lock_configured) {
		libinput_device_config_tap_set_drag_lock_enabled(
			device,
			libinput_config.drag_lock
		);
	}
	
	if (libinput_config.accel_speed_configured) {
		libinput_device_config_accel_set_speed(
			device,
			libinput_config.accel_speed
		);
	}
	
	if (libinput_config.accel_profile_configured) {
		libinput_device_config_accel_set_profile(
			device,
			libinput_config.accel_profile
		);
	}
	
	if (libinput_config.natural_scroll_configured) {
		libinput_device_config_scroll_set_natural_scroll_enabled(
			device,
			libinput_config.natural_scroll
		);
	}
	
	if (libinput_config.left_handed_configured) {
		libinput_device_config_left_handed_set(
			device,
			libinput_config.left_handed
		);
	}
	
	if (libinput_config.click_method_configured) {
		libinput_device_config_click_set_method(
			device,
			libinput_config.click_method
		);
	}
	
	if (libinput_config.middle_emulation_configured) {
		libinput_device_config_middle_emulation_set_enabled(
			device,
			libinput_config.middle_emulation
		);
	}
	
	if (libinput_config.scroll_method_configured) {
		libinput_device_config_scroll_set_method(
			device,
			libinput_config.scroll_method
		);
	}
	
	if (libinput_config.scroll_button_configured) {
		libinput_device_config_scroll_set_button(
			device,
			libinput_config.scroll_button
		);
	}
	
	if (libinput_config.dwt_configured) {
		libinput_device_config_dwt_set_enabled(
			device,
			libinput_config.dwt
		);
	}
}
