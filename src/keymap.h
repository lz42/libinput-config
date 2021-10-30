#ifndef KEYMAP_H
#define KEYMAP_H

#include <stdbool.h>
#include <stdint.h>

void libinput_keymap_init(void);
bool libinput_keymap_set(uint32_t, uint32_t);
uint32_t libinput_keymap_transform(uint32_t);

#endif
