#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

#include <os/newlib/syscalls.h>

static char output[80];

int uprintf(const char *fmt, ...) {
    
    int ret;
    va_list ap;
    
    va_start(ap, fmt);
    ret = vsnprintf(output, sizeof(output), fmt, ap);
    write(0, output, ret);
    va_end(ap);
    return ret;
}

int uvprintf(const char *fmt, va_list ap) {
    int ret;
    ret = vsnprintf(output, sizeof(output), fmt, ap);
    write(0, output, ret);
    return ret;
}

int ugetchar(void) {
    uint8_t byte;
    read(0, &byte, sizeof(byte));
    return (int)byte;
}

int uputchar(int c) {
    uint8_t byte = (uint8_t)c;
    write(0, &byte, sizeof(byte));
    return c;
}

int uputs(const char *s) {
    int size = strlen(s);
    write(0, s, size + 1);
    return size;
}