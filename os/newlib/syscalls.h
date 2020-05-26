#ifndef OS_NEWLIB_SYSCALLS_H_
#define OS_NEWLIB_SYSCALLS_H_

#include <sys/types.h>

extern int _close(int fd);

extern ssize_t read(int fd, void *data, size_t size);

extern ssize_t write(int fd, const void *data, size_t size);

extern off_t lseek(int fd, off_t offset, int whence);

#endif // OS_NEWLIB_SYSCALLS_H_
