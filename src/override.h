#ifndef OVERRIDE_H
#define OVERRIDE_H

#include <libinput.h>

struct libinput_real {
	enum libinput_config_status (*tap_set_enabled)(
		struct libinput_device *,
		enum libinput_config_tap_state
	);
	enum libinput_config_status (*tap_set_button_map)(
		struct libinput_device *,
		enum libinput_config_tap_button_map
	);
	enum libinput_config_status (*tap_set_drag_enabled)(
		struct libinput_device *,
		enum libinput_config_drag_state
	);
	enum libinput_config_status (*tap_set_drag_lock_enabled)(
		struct libinput_device *,
		enum libinput_config_drag_lock_state
	);
	enum libinput_config_status (*accel_set_speed)(
		struct libinput_device *,
		double
	);
	enum libinput_config_status (*accel_set_profile)(
		struct libinput_device *,
		enum libinput_config_accel_profile
	);
	enum libinput_config_status (*scroll_set_natural_scroll_enabled)(
		struct libinput_device *,
		int
	);
	enum libinput_config_status (*left_handed_set)(
		struct libinput_device *,
		int
	);
	enum libinput_config_status (*click_set_method)(
		struct libinput_device *,
		enum libinput_config_click_method
	);
	enum libinput_config_status (*middle_emulation_set_enabled)(
		struct libinput_device *,
		enum libinput_config_middle_emulation_state
	);
	enum libinput_config_status (*scroll_set_method)(
		struct libinput_device *,
		enum libinput_config_scroll_method
	);
	enum libinput_config_status (*scroll_set_button)(
		struct libinput_device *,
		uint32_t
	);
	enum libinput_config_status (*dwt_set_enabled)(
		struct libinput_device *,
		enum libinput_config_dwt_state
	);
};

extern struct libinput_real libinput_real;

void libinput_real_init(void);

#endif
