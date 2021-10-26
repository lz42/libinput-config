#ifndef WORKAROUND_H
#define WORKAROUND_H

#pragma weak libinput_device_get_name
#pragma weak libinput_event_get_device
#pragma weak libinput_event_get_type

#if LIBINPUT_SHITTY_SANDBOXING && !LIBINPUT_NON_GLIBC

asm (".symver dlsym, dlsym@GLIBC_2.2.5");

#endif

#endif
