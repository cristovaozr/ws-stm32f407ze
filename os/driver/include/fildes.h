#ifndef OS_DRIVER_INCLUDE_FILDES_H_
#define OS_DRIVER_INCLUDE_FILDES_H_

#include <sys/types.h>
#include <FreeRTOS.h>
#include <queue.h>
#include <semphr.h>

struct f_ops;

/**
 * @brief Defines a struct fildes
 */
struct fildes {
    int fd;
    const struct f_ops *fops;
    QueueHandle_t tx_queue;
    QueueHandle_t rx_queue;
    SemaphoreHandle_t mutex;
    void *internal_device;
};


/**
 * @brief Basic operations on files
 */
struct f_ops {
    int         (*init)(struct fildes *);
    int         (*fini)(struct fildes *);
    int         (*open)(struct fildes *);
    int         (*close)(struct fildes *);
    ssize_t     (*read)(struct fildes *, void *, size_t);
    ssize_t     (*write)(struct fildes *, const void *, size_t);
    off_t       (*lseek)(struct fildes*, off_t, int);
};

/**
 * @brief Obtains a fildes object from a file descriptor number
 *
 * @param fd 
 * @return struct fildes* const 
 */
extern struct fildes * const obtain_fildes(int fd);

extern int store_fildes(struct fildes *fildes);

#endif // OS_DRIVER_INCLUDE_FILDES_H_
