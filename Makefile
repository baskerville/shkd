VERSION = 0.1

CC      = gcc
LIBS    = -lm
CFLAGS  = -std=gnu99 -pedantic -Wall -Wextra -DVERSION=\"$(VERSION)\"
LDFLAGS = $(LIBS)

PREFIX    ?= /usr/local
BINPREFIX = $(PREFIX)/bin
MANPREFIX = $(PREFIX)/share/man
SYSDPREFIX = /usr/lib/systemd/system
INPUT_DEVICE = $(shell find /dev/input/by-{id,path} -name '*-kbd' | head -n 1)

SRC = shkd.c keycodes.c helpers.c
OBJ = $(SRC:.c=.o)

all: CFLAGS += -Os
all: LDFLAGS += -s
all: options shkd

debug: CFLAGS += -O0 -g -DDEBUG
debug: options shkd

options:
	@echo "shkd build options:"
	@echo "CC      = $(CC)"
	@echo "CFLAGS  = $(CFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"
	@echo "PREFIX  = $(PREFIX)"

.c.o:
	@echo "CC $<"
	@$(CC) $(CFLAGS) -DVERSION=\"$(VERSION)\" -c -o $@ $<

shkd: $(OBJ)
	@echo CC -o $@
	@$(CC) -o $@ $(OBJ) $(LDFLAGS)

clean:
	@echo "cleaning"
	@rm -f $(OBJ) shkd

install:
	@echo "installing executable files to $(DESTDIR)$(BINPREFIX)/shkd"
	@install -D -m 755 shkd $(DESTDIR)$(BINPREFIX)/shkd
	@echo "installing manual page to $(DESTDIR)$(MANPREFIX)/man1/shkd.1"
	@install -D -m 644 shkd.1 $(DESTDIR)$(MANPREFIX)/man1/shkd.1
	@echo "installing configuration file to $(DESTDIR)/etc/shkdrc"
	@install -D -m 644 examples/shkdrc $(DESTDIR)/etc/shkdrc
	@install -D -m 644 examples/98-shk-local.rules $(DESTDIR)/etc/udev/rules.d/98-shk-local.rules
	@echo /etc/udev/rules.d/98-shk-local.rules is launching shkd on all kbd input devices
	@echo "installing systemd service file to $(DESTDIR)$(SYSDPREFIX)/shkd@.service"
	@sed "s#BIN_PREFIX#$(BINPREFIX)#" examples/shkd.service.template > examples/shkd@.service
	@install -D -m 644 examples/shkd@.service $(DESTDIR)$(SYSDPREFIX)/shkd@.service

uninstall:
	@echo "removing executable files from $(DESTDIR)$(BINPREFIX)/shkd"
	@rm -f $(DESTDIR)$(BINPREFIX)/shkd
	@echo "removing manual page from $(DESTDIR)$(MANPREFIX)/man1/shkd.1"
	@rm -f $(DESTDIR)$(MANPREFIX)/man1/shkd.1
	@echo "removing configuration file from $(DESTDIR)/etc/shkdrc"
	@rm -f $(DESTDIR)/etc/shkdrc
	@rm -f $(DESTDIR)/etc/udev/rules.d/98-shk-local.rules
	@echo "removing systemd service file from $(DESTDIR)$(SYSDPREFIX)/shkd@.service"
	@rm -f $(DESTDIR)$(SYSDPREFIX)/shkd@.service

.PHONY: all debug options clean install uninstall
