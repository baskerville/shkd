## Usage

    SYNOPSIS
        shkd [OPTIONS] INPUT_DEVICE

    OPTIONS
        -h
            Show the synopsis and quit.

        -v
            Show the version and quit.

        -t
            Output the name and the keycode of the incoming key press events (press 'esc' to quit).

        -c CONFIG_FILE
            Read configuration from the given file instead of the default (/etc/shkdrc).

## Configuration File Format

Each line of the configuration file is interpreted as so:

- If it starts with `#`, it is ignored.
- If it starts with one or more white space characters (cf. `man 3 isspace`) it is read as a command.
- Otherwise, it is parsed as a hotkey: each key name is separated by spaces or `+` characters.

## Installation

    make
    make install
