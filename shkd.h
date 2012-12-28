#ifndef _LHKD_H
#define _LHKD_H

#include <stdio.h>

#define CONFIG_FILE  "/etc/shkdrc"
#define TOK_SEP      "+ \n"

#define VA_RELEASE  0
#define VA_PRESS    1
#define VA_REPEAT   2

typedef struct hotkey_t hotkey_t;
struct hotkey_t {
    unsigned char modmask;
    unsigned short keycode;
    char command[BUFSIZ];
    hotkey_t *prev;
};

typedef struct {
    const char *name;
    unsigned short keycode;
} reg_key_t;

typedef struct {
    const char *name;
    unsigned short keycode;
    unsigned char modval;
} mod_key_t;

hotkey_t *hotkeys;
char *config_file;
bool care_for[KEY_CNT];
bool test_mode, running;

unsigned char modmask;
reg_key_t rk;
mod_key_t mk;

void stop(int);
void init(void);
void cleanup(void);
void load_config(void);
void handle_event(struct input_event *);

#endif
