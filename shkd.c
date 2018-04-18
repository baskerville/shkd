#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <ctype.h>
#include <getopt.h>
#include <signal.h>
#include <stdbool.h>
#include <errno.h>
#include "helpers.h"
#include "keycodes.h"
#include "shkd.h"

void hold(int sig)
{
    if (sig == SIGHUP || sig == SIGINT || sig == SIGTERM)
        running = false;
    else if (sig == SIGUSR1)
        reload = true;
}

void init(void)
{
    hotkeys = NULL;
    modmask = 0;
    current_mode = NULL;
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

/* http://lubutu.com/code/spawning-in-unix */
int spawnv(const char *file, char *const argv[])
{
    int fd[2];
    if (pipe(fd) == -1)
        return -1;
    switch(fork()) {
        case 0:
            switch (fork()) {
                case 0:
                    close(fd[0]);
                    if (fcntl(fd[1], F_SETFD, FD_CLOEXEC) == 0)
                        execvp(file, argv);
                    /* fallthrough */
                case -1:
                    write(fd[1], &errno, sizeof errno);
                    exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        case -1:
            close(fd[0]);
            close(fd[1]);
            return -1;
    }
    wait(NULL);
    close(fd[1]);
    if (read(fd[0], &errno, sizeof errno)) {
        close(fd[0]);
        return -1;
    }
    close(fd[0]);
    return 0;
}

void load_config(void)
{
    if (hotkeys != NULL) {
        cleanup();
        init();
    }

    FILE *cfg = fopen((config_file == NULL ? CONFIG_FILE : config_file), "r");
    if (cfg == NULL)
        err("can't open configuration file\n");

    char line[BUFSIZ];
    reg_key_t rk;
    mod_key_t mk;
    unsigned char moma = 0;
    unsigned short keycode = KEY_RESERVED;
    char *mode = NULL;
    char *next_mode = NULL;

    while (fgets(line, sizeof(line), cfg) != NULL) {
        if (strlen(line) == 1 || line[0] == '#') {
            continue;
        } else if (isspace(line[0])) {
            if (keycode == KEY_RESERVED)
                continue;

            /* trim whitespace from line */
            unsigned int i = strlen(line) - 1;
            while (i > 0 && isspace(line[i]))
                line[i--] = '\0';
            i = 1;
            while (isspace(line[i]))
                i++;

            /* parse new mode */
            unsigned int next_space = i;
            while (line[next_space] && !isspace(line[next_space]))
                next_space++;
            if (line[i]=='-' && line[i+1] == '>' && line[i+2] == '[' && line[next_space-1] == ']') {
                next_mode = strdup(&line[i+3]);
                next_mode[(next_space-i)-4] = '\0';
                /* skip over the new mode and any whitespace */
                i=next_space;
                while (isspace(line[i]))
                    i++;
            } else {
                free(next_mode);
                next_mode = NULL;
            }

            /* save the command */
            if (i < strlen(line)) {
                char *command = line + i;
                hotkey_t *hk = make_hotkey(moma, keycode, mode, next_mode, command);
                hk->prev = hotkeys;
                hotkeys = hk;
            }
            moma = 0;
            keycode = KEY_RESERVED;
        } else {
            char *name = strtok(line, TOK_SEP);
            if (name == NULL)
                continue;

            size_t namelen = strlen(name);

            /* parse mode restriction */
            if (name[0] == '[' && name[namelen-1] == ']') {
                mode = strdup(&name[1]);
                mode[namelen-2] = '\0';
            } else {
                free(mode);
                mode = NULL;
            }

            /* parse modifiers and key */
            do {
                if (reg_key_from_name(name, &rk)) {
                    care_for[rk.keycode] = true;
                    keycode = rk.keycode;
                } else if (mod_key_from_name(name, &mk, care_for)) {
                    moma |= mk.modval;
                }
            } while ((name = strtok(NULL, TOK_SEP)) != NULL);
        }
    }

    fclose(cfg);
}

void handle_event(struct input_event *ev)
{
    if (ev->type == EV_KEY) {
        if (test_mode) {
            if (ev->value == VA_PRESS)
            {
                if (reg_key_from_keycode(ev->code, &reg_key))
                    printf("%s (%u)\n", reg_key.name, reg_key.keycode);
                else if (mod_key_from_keycode(ev->code, &mod_key))
                    printf("%s (%u)\n", mod_key.name, mod_key.keycode);
                else
                    printf("? (%u)\n", ev->code);
                if (ev->code == KEY_ESC)
                    running = false;
            }
            return;
        }

        if (!care_for[ev->code])
            return;

        if (ev->value == VA_PRESS || ev->value == VA_REPEAT || ev->value == VA_RELEASE) {
            /* search for the hotkey */
            hotkey_t *hk = NULL;
            if (ev->value == VA_RELEASE) {
                hk = find_hotkey(modmask | RELEASE_MASK, ev->code, current_mode);
            } else {
                hk = find_hotkey(modmask, ev->code, current_mode);
            }

            if (hk != NULL) {
                /* change mode if the hotkey specifies a new mode */
                if (hk->next_mode != NULL) {
                    current_mode = hk->next_mode;
                }
                /* run the hotkey's command */
                char *cmd[] = {SHELL, "-c", hk->command, NULL};
                if (spawnv(cmd[0], cmd) == -1)
                    warn("failed to execute command: %s\n", hk->command);
            } else if (ev->value == VA_PRESS && mod_key_from_keycode(ev->code, &mod_key)) {
                modmask |= mod_key.modval;
            }
        }

        /* remove released modifiers from the current modmask */
        if (ev->value == VA_RELEASE) {
            if (mod_key_from_keycode(ev->code, &mod_key))
                modmask &= ~mod_key.modval;
        }
    }
}

int main(int argc, char *argv[])
{
    test_mode = false;
    config_file = NULL;
    int opt;

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

    unsigned int num = argc - optind;
    char **input_device = argv + optind;
    int fd[num], sel_fd, max_fd = 0;

    for (unsigned int i = 0; i < num; i++) {
        fd[i] = open(input_device[i], O_RDONLY|O_CLOEXEC);
        if (fd[i] == -1)
            err("can't open input device %s\n", input_device[i]);
        else if (fd[i] > max_fd)
            max_fd = fd[i];
    }

    sel_fd = max_fd + 1;

    signal(SIGINT, hold);
    signal(SIGHUP, hold);
    signal(SIGTERM, hold);
    signal(SIGUSR1, hold);

    init();
    if (!test_mode)
        load_config();

    fd_set descriptors;
    struct input_event ev;

    reload = false;
    running = true;

    while (running) {
        FD_ZERO(&descriptors);
        for (unsigned int i = 0; i < num; i++)
            FD_SET(fd[i], &descriptors);
        if (select(sel_fd, &descriptors, NULL, NULL, NULL) > 0) {
            for (unsigned int i = 0; i < num; i++)
                if (FD_ISSET(fd[i], &descriptors)) {
                    if (read(fd[i], &ev, sizeof(struct input_event)) > 0)
                        handle_event(&ev);
                    else
                        running = false;
                }
        }
        if (reload) {
            reload = false;
            signal(SIGUSR1, hold);
            if (test_mode)
                continue;
            load_config();
        }
    }

    for (unsigned int i = 0; i < num; i++)
        close(fd[i]);
    cleanup();
    return EXIT_SUCCESS;
}
