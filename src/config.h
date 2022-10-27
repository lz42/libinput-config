#ifndef CONFIG_H
#define CONFIG_H

#include <libinput.h>
#include <stdbool.h>

#define xstringify(a) #a
#define stringify(a) xstringify(a)

#define elc(type) enum libinput_config_##type
#define hook(symbol) dlsym(RTLD_NEXT, symbol)

#define config_option(type, name)\
	bool name##_configured;\
	type name

struct libinput_config {
	bool configured;
	
	bool override_compositor;
	
	config_option(elc(tap_state), tap);
	config_option(elc(tap_button_map), tap_button_map);
	config_option(elc(drag_state), drag);
	config_option(elc(drag_lock_state), drag_lock);
	config_option(double, accel_speed);
	config_option(elc(accel_profile), accel_profile);
	config_option(bool, natural_scroll);
	config_option(bool, left_handed);
	config_option(elc(click_method), click_method);
	config_option(elc(middle_emulation_state), middle_emulation);
	config_option(elc(scroll_method), scroll_method);
	config_option(uint32_t, scroll_button);
	config_option(elc(dwt_state), dwt);
	config_option(elc(dwtp_state), dwtp);
	
	double scroll_factor_x;
	double scroll_factor_y;
	
	double discrete_scroll_factor_x;
	double discrete_scroll_factor_y;
	
	double speed_x;
	double speed_y;
	
	double gesture_speed_x;
	double gesture_speed_y;
};

extern struct libinput_config libinput_config;

void libinput_config_init(void);
void libinput_config_device(struct libinput_device *);

#endif
