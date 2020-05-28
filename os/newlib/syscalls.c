#include "driver/include/fildes.h"
#include <sys/types.h>
#include <stdarg.h>

int open(const char *filepath, int flags)
{
    struct fildes * const fildes = obtain_fildes_str(filepath);
    if (fildes == NULL) return -1;
    if (fildes->fops->open == NULL) return -1;
    return fildes->fops->open(fildes, flags);
}

int close(int fd)
{
    struct fildes * const fildes = obtain_fildes(fd);
    if (fildes == NULL) return -1;
    if (fildes->fops->close == NULL) return -1;
    return fildes->fops->close(fildes);
}

ssize_t read(int fd, void *data, size_t size)
{
    struct fildes * const fildes = obtain_fildes(fd);
    if (fildes->fops->read == NULL) return -1;
    if (fildes == NULL) return -1;

    return fildes->fops->read(fildes, data, size);
}

ssize_t write(int fd, const void *data, size_t size)
{
    struct fildes * const fildes = obtain_fildes(fd);
    if (fildes == NULL) return -1;
    if (fildes->fops->write == NULL) return -1;
    return fildes->fops->write(fildes, data, size);
}

off_t lseek(int fd, off_t offset, int whence)
{
    struct fildes * const fildes = obtain_fildes(fd);
    if (fildes == NULL) return -1;
    if (fildes->fops->lseek == NULL) return -1;
    return fildes->fops->lseek(fildes, offset, whence);
}

int ioctl(int fd, unsigned long request, ...)
{
    va_list ap;
    struct fildes * const fildes = obtain_fildes(fd);
    if (fildes == NULL) return -1;
    if (fildes->fops->ioctl == NULL) return -1;

    va_start(ap, request);
    long data = va_arg(ap, long);
    int ret = fildes->fops->ioctl(fildes, request, data);
    va_end(ap);

    return ret;
}