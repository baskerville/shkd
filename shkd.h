#ifndef _SHKD_H
#define _SHKD_H

#include <stdio.h>

#define CONFIG_FILE  "/etc/shkdrc"
#define TOK_SEP      "+ \n"

#define VA_RELEASE  0
#define VA_PRESS    1
#define VA_REPEAT   2

#define RELEASE_MASK (1<<6)

typedef struct hotkey_t hotkey_t;
struct hotkey_t {
    unsigned char modmask;
    unsigned short keycode;
    char* mode;
    char* next_mode;
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
bool running, test_mode, reload;

const char *current_mode;
unsigned char modmask;
reg_key_t reg_key;
mod_key_t mod_key;

void hold(int);
void init(void);
void cleanup(void);
void load_config(void);
void handle_event(struct input_event *);

#endif
