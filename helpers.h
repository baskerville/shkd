#ifndef _HELPERS_H
#define _HELPERS_H

#define LENGTH(x)         (sizeof(x) / sizeof(*x))
#define SHELL             "/bin/sh"

void warn(char *, ...);
__attribute__((noreturn))
void err(char *, ...);

#endif
