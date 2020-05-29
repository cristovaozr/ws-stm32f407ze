#include "driver/include/fildes.h"
#include "libc/log.h"
#include <sys/types.h>
#include <stdarg.h>

int open(const char *filepath, int flags)
{
    int ret;
    struct fildes * const fildes = obtain_fildes_str(filepath);
    if (fildes == NULL) {
        ERROR("syscall", "fildes not found for: %s", filepath);
        ret = -1;
        goto exit;
    }

    if (fildes->fops->open == NULL) {
        ERROR("syscall", "open() call unimplemented for fildes [%p]", fildes);
        return -1;
    }

    ret = fildes->fops->open(fildes, flags);
    if (ret) {
        ERROR("syscall", "Problem opening file %s", filepath);
        ret = -1;
        goto exit;
    }
    ret = fildes->fd;

exit:
    return ret;
}

int close(int fd)
{
    int ret;
    struct fildes * const fildes = obtain_fildes(fd);
    if (fildes == NULL) {
        ERROR("syscall", "fd specified not foud: [%d", fd);
        ret = -1;
        goto exit;
    }

    if (fildes->fops->close == NULL) {
        ERROR("syscall", "close() call unimplemented for fildes [%p]", fildes);
        ret = -1;
        goto exit;
    }

    ret = fildes->fops->close(fildes);
    if (ret) {
        ERROR("syscall", "Problem closing file fd=[%d]", fd);
        ret = -1;
        goto exit;
    }
    ret = 0;

exit:
    return ret;
}

ssize_t read(int fd, void *data, size_t size)
{
    ssize_t ret;
    struct fildes * const fildes = obtain_fildes(fd);
    if (fildes == NULL) {
        ERROR("syscall", "fd specified not foud: [%d", fd);
        ret = -1;
        goto exit;
    }

    if (fildes->fops->read == NULL) {
        ERROR("syscall", "read() call unimplemented for fildes [%p]", fildes);
        ret = -1;
        goto exit;
    }
    ret = fildes->fops->read(fildes, data, size);

exit:
    return ret;
}

ssize_t write(int fd, const void *data, size_t size)
{
    ssize_t ret;
    struct fildes * const fildes = obtain_fildes(fd);
    if (fildes == NULL) {
        ERROR("syscall", "fd specified not foud: [%d", fd);
        ret = -1;
        goto exit;
    }

    if (fildes->fops->write == NULL) {
        ERROR("syscall", "write() call unimplemented for fildes [%p]", fildes);
        ret = -1;
        goto exit;
    }
    ret = fildes->fops->write(fildes, data, size);

exit:
    return ret;
}

off_t lseek(int fd, off_t offset, int whence)
{
    off_t ret;
    struct fildes * const fildes = obtain_fildes(fd);
    if (fildes == NULL) {
        ERROR("syscall", "fd specified not foud: [%d", fd);
        ret = (off_t)-1;
        goto exit;
    }

    if (fildes->fops->lseek == NULL) {
        ERROR("syscall", "lseek() call unimplemented for fildes [%p]", fildes);
        ret = (off_t)-1;
        goto exit;
    }
    ret = fildes->fops->lseek(fildes, offset, whence);

exit:
    return ret;
}

int ioctl(int fd, unsigned long request, ...)
{
    va_list ap;
    int ret;
    struct fildes * const fildes = obtain_fildes(fd);
       if (fildes == NULL) {
        ERROR("syscall", "fd specified not foud: [%d", fd);
        ret = -1;
        goto exit;
    }

    if (fildes->fops->ioctl == NULL) {
        ERROR("syscall", "ioctl() call unimplemented for fildes [%p]", fildes);
        ret = -1;
        goto exit;
    }
    va_start(ap, request);
    long data = va_arg(ap, long);
    ret = fildes->fops->ioctl(fildes, request, data);
    va_end(ap);

exit:
    return ret;
}
