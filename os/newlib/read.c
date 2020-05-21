#include <sys/types.h>
#include <driver/include/fildes.h>

ssize_t _read(int fd, void *data, size_t size) {
    FileDescriptor * const fildes = ObtainFileDescriptor(fd);
    if(fildes == NULL) return -1;

    return fildes->fileOperations->read(fildes, data, size);
}