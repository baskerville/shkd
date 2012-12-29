#ifndef _KEYCODES_H
#define _KEYCODES_H

#include "shkd.h"

bool reg_key_from_name(const char *, reg_key_t *);
bool reg_key_from_keycode(unsigned short, reg_key_t *);
bool mod_key_from_name(const char *, mod_key_t *, bool *);
bool mod_key_from_keycode(unsigned short, mod_key_t *);
hotkey_t *find_hotkey(unsigned char, unsigned short);
hotkey_t *make_hotkey(unsigned char, unsigned short, char *);

#endif
