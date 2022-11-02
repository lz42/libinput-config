#ifndef CONFIG_H
#define CONFIG_H

#include <libinput.h>
#include <stdbool.h>
#include <stdio.h>

#pragma weak libinput_device_get_name
#pragma weak libinput_event_get_device
#pragma weak libinput_event_get_type

#if LIBINPUT_SHITTY_SANDBOXING && !LIBINPUT_NON_GLIBC

asm (".symver dlsym, dlsym@GLIBC_2.2.5");

#endif

#define print(...)\
	fprintf(stderr, "libinput-config: ");\
	fprintf(stderr, __VA_ARGS__);\
	fprintf(stderr, "\n");\

#define xstringify(a) #a
#define stringify(a) xstringify(a)

#define elc(type) enum libinput_config_##type
#define hook(symbol) dlsym(RTLD_NEXT, symbol)

#define config_option(type, name)\
	bool name##_configured;\
	type name

#define config_function(name, type)\
	enum libinput_config_status (*name)(\
		struct libinput_device *,\
		type\
	)

struct libinput_config {
	bool configured;
	
	bool override_compositor;
	
	config_option(elc(tap_state), tap);
	config_option(elc(tap_button_map), tap_button_map);
	config_option(elc(drag_state), drag);
	config_option(elc(drag_lock_state), drag_lock);
	config_option(elc(scroll_button_lock_state), scroll_button_lock);
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

struct libinput_real {
	config_function(tap_set_enabled, elc(tap_state));
	config_function(tap_set_button_map, elc(tap_button_map));
	config_function(tap_set_drag_enabled, elc(drag_state));
	config_function(tap_set_drag_lock_enabled, elc(drag_lock_state));
	config_function(scroll_set_button_lock_enabled, elc(scroll_button_lock_state));
	config_function(accel_set_speed, double);
	config_function(accel_set_profile, elc(accel_profile));
	config_function(scroll_set_natural_scroll_enabled, int);
	config_function(left_handed_set, int);
	config_function(click_set_method, elc(click_method));
	config_function(middle_emulation_set_enabled, elc(middle_emulation_state));
	config_function(scroll_set_method, elc(scroll_method));
	config_function(scroll_set_button, uint32_t);
	config_function(dwt_set_enabled, elc(dwt_state));
	config_function(dwtp_set_enabled, elc(dwtp_state));
};

extern struct libinput_config libinput_config;
extern struct libinput_real libinput_real;

void libinput_config_init(void);
void libinput_config_device(struct libinput_device *);

void libinput_real_init(void);

void libinput_keymap_init(void);
bool libinput_keymap_set(uint32_t, uint32_t);
uint32_t libinput_keymap_transform(uint32_t);

#endif
