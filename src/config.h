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

#define config_option(type, name, function)\
	bool name##_configured;\
	type name;\
	\
	enum libinput_config_status (*function)(\
		struct libinput_device *,\
		type\
	)

struct libinput_config {
	bool configured;
	
	bool override_compositor;
	
	config_option(elc(tap_state), tap, tap_set_enabled);
	config_option(elc(tap_button_map), tap_button_map, tap_set_button_map);
	config_option(elc(drag_state), drag, tap_set_drag_enabled);
	config_option(elc(drag_lock_state), drag_lock, tap_set_drag_lock_enabled);
	config_option(elc(scroll_button_lock_state), scroll_button_lock, scroll_set_button_lock_enabled);
	config_option(double, accel_speed, accel_set_speed);
	config_option(elc(accel_profile), accel_profile, accel_set_profile);
	config_option(bool, natural_scroll, scroll_set_natural_scroll_enabled);
	config_option(bool, left_handed, left_handed_set);
	config_option(elc(click_method), click_method, click_set_method);
	config_option(elc(middle_emulation_state), middle_emulation, middle_emulation_set_enabled);
	config_option(elc(scroll_method), scroll_method, scroll_set_method);
	config_option(uint32_t, scroll_button, scroll_set_button);
	config_option(elc(dwt_state), dwt, dwt_set_enabled);
	config_option(elc(dwtp_state), dwtp, dwtp_set_enabled);
	
	double scroll_factor_x;
	double scroll_factor_y;
	
	double discrete_scroll_factor_x;
	double discrete_scroll_factor_y;
	
	double speed_x;
	double speed_y;
	
	double gesture_speed_x;
	double gesture_speed_y;
};

#undef config_option

extern struct libinput_config libinput_config;

void libinput_config_init(void);
void libinput_config_device(struct libinput_device *);

void libinput_real_init(void);

void libinput_keymap_init(void);
bool libinput_keymap_set(uint32_t, uint32_t);
uint32_t libinput_keymap_transform(uint32_t);

#endif
