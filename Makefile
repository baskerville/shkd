VERSION = 0.1

CC      = gcc
LIBS    = -lm
CFLAGS  = -std=c99 -pedantic -Wall -Wextra -DVERSION=\"$(VERSION)\"
LDFLAGS = $(LIBS)

PREFIX    ?= /usr/local
BINPREFIX = $(PREFIX)/bin
SYSDPREFIX = /usr/lib/systemd/system
INPUT_DEVICE = $(shell find /dev/input/by-id/ -name '*-kbd' | head -n 1)

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
	@echo "installing executable files to $(DESTDIR)$(BINPREFIX)"
	@install -D -m 755 shkd $(DESTDIR)$(BINPREFIX)/shkd
	@echo "installing configuration file to $(DESTDIR)/etc"
	@install -D -m 644 examples/shkdrc $(DESTDIR)/etc/shkdrc
	@echo "installing systemd service file to $(DESTDIR)$(SYSDPREFIX)"
	@sed "s:INPUT_DEVICE:$(INPUT_DEVICE):;s:BIN_PREFIX:$(BINPREFIX):" examples/shkd.service.template > examples/shkd.service
	@install -D -m 644 examples/shkd.service $(DESTDIR)$(SYSDPREFIX)/shkd.service

uninstall:
	@echo "removing executable files from $(DESTDIR)$(BINPREFIX)"
	@rm -f $(DESTDIR)$(BINPREFIX)/shkd
	@echo "removing configuration file from $(DESTDIR)/etc"
	@rm -f $(DESTDIR)/etc/shkdrc
	@echo "removing systemd service file from $(DESTDIR)$(SYSDPREFIX)"
	@rm -f $(DESTDIR)$(SYSDPREFIX)/shkd.service

.PHONY: all debug options clean install uninstall
