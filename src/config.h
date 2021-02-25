#ifndef CONFIG_H
#define CONFIG_H

#include <libinput.h>
#include <stdio.h>
#include <stdbool.h>

#define print(...)\
	fprintf(stderr, "libinput-config: ");\
	fprintf(stderr, __VA_ARGS__);\
	fprintf(stderr, "\n");\

struct libinput_config {
	bool configured;
	
	bool tap_configured;
	enum libinput_config_tap_state tap;
	
	bool tap_button_map_configured;
	enum libinput_config_tap_button_map tap_button_map;
	
	bool drag_configured;
	enum libinput_config_drag_state drag;
	
	bool drag_lock_configured;
	enum libinput_config_drag_lock_state drag_lock;
	
	bool accel_speed_configured;
	double accel_speed;
	
	bool accel_profile_configured;
	enum libinput_config_accel_profile accel_profile;
	
	bool natural_scroll_configured;
	bool natural_scroll;
	
	bool left_handed_configured;
	bool left_handed;
	
	bool click_method_configured;
	enum libinput_config_click_method click_method;
	
	bool middle_emulation_configured;
	enum libinput_config_middle_emulation_state middle_emulation;
	
	bool scroll_method_configured;
	enum libinput_config_scroll_method scroll_method;
	
	bool scroll_button_configured;
	uint32_t scroll_button;
	
	bool dwt_configured;
	enum libinput_config_dwt_state dwt;
	
	double scroll_factor;
	double discrete_scroll_factor;
	double speed;
};

extern struct libinput_config libinput_config;

void libinput_config_init(void);
void libinput_config_device(struct libinput_device *);

#endif
