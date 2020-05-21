#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

#include <os/newlib/syscalls.h>

int printf(const char *fmt, ...) {
    char output[80];
    int ret;
    va_list ap;
    
    va_start(ap, fmt);
    ret = vsnprintf(output, sizeof(output), fmt, ap);
    _write(0, output, ret);
    va_end(ap);
    return ret;
}

int getchar(void) {
    uint8_t byte;
    _read(0, &byte, sizeof(byte));
    return (int)byte;
}

int putchar(int c) {
    uint8_t byte = (uint8_t)c;
    _write(0, &byte, sizeof(byte));
    return c;
}