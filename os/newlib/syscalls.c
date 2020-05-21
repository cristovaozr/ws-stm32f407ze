#include <sys/types.h>
#include <driver/include/fildes.h>

int _close(int fd) {
    FileDescriptor * const fildes = ObtainFileDescriptor(fd);
    if(fildes == NULL) return -1;
    if(fildes->fileOperations->close == NULL) return -1;
    return fildes->fileOperations->close(fildes);
}

ssize_t _read(int fd, void *data, size_t size) {
    FileDescriptor * const fildes = ObtainFileDescriptor(fd);
    if(fildes->fileOperations->read == NULL) return -1;
    if(fildes == NULL) return -1;

    return fildes->fileOperations->read(fildes, data, size);
}

ssize_t _write(int fd, const void *data, size_t size) {
    FileDescriptor * const fildes = ObtainFileDescriptor(fd);
    if(fildes == NULL) return -1;
    if(fildes->fileOperations->write == NULL) return -1;
    return fildes->fileOperations->write(fildes, data, size);
}

off_t _lseek(int fd, off_t offset, int whence) {
    FileDescriptor * const fildes = ObtainFileDescriptor(fd);
    if(fildes == NULL) return -1;
    if(fildes->fileOperations->lseek == NULL) return -1;
    return fildes->fileOperations->lseek(fildes, offset, whence);
}

