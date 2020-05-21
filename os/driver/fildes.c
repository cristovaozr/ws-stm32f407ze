#include <driver/include/fildes.h>

#include <string.h>

// TODO: increase files array size
static FileDescriptor *files[1];

#define ARRAY_SIZE(x) (sizeof((x))/sizeof((x)[0]))

FileDescriptor * const ObtainFileDescriptor(int fd)
{
    for(int i = 0; i < ARRAY_SIZE(files); i++) {
        if(files[i]->fd == fd)  return files[i];
    }

    return NULL;
}

int StoreFileDescriptor(FileDescriptor *fileDescriptor)
{
    int ret = -1;

    for(int i = 0; i < ARRAY_SIZE(files); i++) {
        if(files[i] == NULL) {
            if (fileDescriptor->fileOperations->init(fileDescriptor)) {
                // TODO: Signal something went wrong
                break;
            }
            fileDescriptor->fd = i;
            files[i] = fileDescriptor;
            ret = 0;
            break;
        }
    }

    return ret;
}
