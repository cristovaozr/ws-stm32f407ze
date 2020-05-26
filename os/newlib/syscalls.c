#include <sys/types.h>
#include <driver/include/fildes.h>

int _close(int fd) {
    struct fildes * const fildes = obtain_fildes(fd);
    if(fildes == NULL) return -1;
    if(fildes->fops->close == NULL) return -1;
    return fildes->fops->close(fildes);
}

ssize_t _read(int fd, void *data, size_t size) {
    struct fildes * const fildes = obtain_fildes(fd);
    if(fildes->fops->read == NULL) return -1;
    if(fildes == NULL) return -1;

    return fildes->fops->read(fildes, data, size);
}

ssize_t _write(int fd, const void *data, size_t size) {
    struct fildes * const fildes = obtain_fildes(fd);
    if(fildes == NULL) return -1;
    if(fildes->fops->write == NULL) return -1;
    return fildes->fops->write(fildes, data, size);
}

off_t _lseek(int fd, off_t offset, int whence) {
    struct fildes * const fildes = obtain_fildes(fd);
    if(fildes == NULL) return -1;
    if(fildes->fops->lseek == NULL) return -1;
    return fildes->fops->lseek(fildes, offset, whence);
}
