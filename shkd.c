#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <getopt.h>
#include <signal.h>
#include <stdbool.h>
#include "helpers.h"
#include "keycodes.h"
#include "shkd.h"

void stop(int sig)
{
    if (sig == SIGHUP || sig == SIGINT || sig == SIGTERM)
        running = false;
}

void init(void)
{
    config_file = NULL;
    hotkeys = NULL;
    running = true;
    for (unsigned int i = 0; i < LENGTH(care_for); i++)
        care_for[i] = false;
}

void cleanup(void)
{
    hotkey_t *hk = hotkeys;
    while (hk != NULL) {
        hotkey_t *tmp = hk->prev;
        free(hk);
        hk = tmp;
    }
}

void load_config(void)
{
    FILE *cfg = fopen((config_file == NULL ? CONFIG_FILE : config_file), "r");
    if (cfg == NULL)
        err("can't open configuration file");

    char line[BUFSIZ];
    reg_key_t k;
    mod_key_t m;
    unsigned char modmask = 0;
    unsigned short keycode = KEY_RESERVED;

    while (fgets(line, sizeof(line), cfg) != NULL) {
        if (strlen(line) == 1 || line[0] == '#') {
            continue;
        } else if (isspace(line[0])) {
            if (keycode == KEY_RESERVED)
                continue;
            unsigned int i = strlen(line) - 1;
            while (i > 0 && isspace(line[i]))
                line[i--] = '\0';
            i = 1;
            while (i < strlen(line) && isspace(line[i]))
                i++;
            if (i < strlen(line)) {
                char *command = line + i;
                hotkey_t *hk = make_hotkey(modmask, keycode, command);
                hk->prev = hotkeys;
                hotkeys = hk;
            }
            modmask = 0;
            keycode = KEY_RESERVED;
        } else {
            char *name = strtok(line, TOK_SEP);
            if (name == NULL)
                continue;
            do {
                if (reg_key_from_name(name, &k)) {
                    care_for[k.keycode] = true;
                    keycode = k.keycode;
                } else if (mod_key_from_name(name, &m)) {
                    care_for[m.keycode] = true;
                    modmask |= m.modval;
                }
            } while ((name = strtok(NULL, TOK_SEP)) != NULL);
        }
    }

    fclose(cfg);
}

int main(int argc, char *argv[])
{
    bool test_mode = false;
    char opt;

    while ((opt = getopt(argc, argv, "vhtc:")) != -1) {
        switch (opt) {
            case 'c':
                config_file = optarg;
                break;
            case 't':
                test_mode = true;
                break;
            case 'v':
                printf("%s\n", VERSION);
                exit(EXIT_SUCCESS);
                break;
            case 'h':
                printf("%s [OPTION] INPUT_DEVICE\n", argv[0]);
                exit(EXIT_SUCCESS);
                break;
        }
    }

    if (optind == argc)
        err("required argument missing\n");

    struct input_event ev;
    char *input_device = argv[optind];

    int fd = open(input_device, O_RDONLY);
    if (fd == -1)
        err("can't open input device\n");

    signal(SIGINT, stop);
    signal(SIGHUP, stop);
    signal(SIGTERM, stop);

    init();
    if (!test_mode)
        load_config();

    unsigned char modmask = 0;
    hotkey_t *hk;
    reg_key_t k;
    mod_key_t m;

    while (running && read(fd, &ev, sizeof(struct input_event)) > 0) {
        if (ev.type == EV_KEY) {
            if (test_mode) {
                if (ev.value == VA_PRESS)
                {
                    if (reg_key_from_keycode(ev.code, &k))
                        printf("%s (%u)\n", k.name, k.keycode);
                    else if (mod_key_from_keycode(ev.code, &m))
                        printf("%s (%u)\n", m.name, m.keycode);
                    else
                        printf("? (%u)\n", ev.code);
                    if (ev.code == KEY_ESC)
                        running = false;
                }
                continue;
            }

            if (!care_for[ev.code])
                continue;

            if (ev.value == VA_PRESS || ev.value == VA_REPEAT) {
                hk = find_hotkey(modmask, ev.code);
                if (hk != NULL) {
                    if (system(hk->command) == -1)
                        warn("failed to execute command: %s\n", hk->command);
                } else if (ev.value == VA_PRESS && mod_key_from_keycode(ev.code, &m)) {
                    modmask |= m.modval;
                }
            } else if (ev.value == VA_RELEASE) {
                if (mod_key_from_keycode(ev.code, &m))
                    modmask &= ~m.modval;
            }
        }
    }

    close(fd);
    cleanup();
    return EXIT_SUCCESS;
}
