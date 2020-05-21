#ifndef OS_DRIVER_INCLUDE_FILDES_H_
#define OS_DRIVER_INCLUDE_FILDES_H_

#include <sys/types.h>
#include <FreeRTOS.h>
#include <queue.h>
#include <semphr.h>

typedef struct FileOperations FileOperations;

/**
 * @brief Defines a FileDescriptor
 */
typedef struct FileDescriptor {
    int fd;
    const FileOperations *fileOperations;
    QueueHandle_t txQueue;
    QueueHandle_t rxQueue;
    SemaphoreHandle_t mutex;
    void *internalDevice;
} FileDescriptor;


/**
 * @brief Basic operations on files
 */
typedef struct FileOperations {
    int         (*init)(FileDescriptor *);
    int         (*fini)(FileDescriptor *);
    int         (*open)(FileDescriptor *);
    int         (*close)(FileDescriptor *);
    ssize_t     (*read)(FileDescriptor *, void *, size_t);
    ssize_t     (*write)(FileDescriptor *, const void *, size_t);
    off_t       (*lseek)(FileDescriptor*, off_t, int);
} FileOperations;

/**
 * @brief Obtains a FileDescriptor object from a file descriptor number
 *
 * @param fd 
 * @return FileDescriptor* const 
 */
extern FileDescriptor * const ObtainFileDescriptor(int fd);

extern int StoreFileDescriptor(FileDescriptor *fileDescriptor);

#endif // OS_DRIVER_INCLUDE_FILDES_H_
